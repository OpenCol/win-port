#!/usr/bin/env python3
"""Link whatever is ported into a real Windows 3.1 executable.

WHY NOW, WITH 128 OF 3,173 FUNCTIONS. The link is the only check that covers what the
differential cannot: that the pieces FIT -- one definition each, the segments inside their
64 KiB, DGROUP not overflowing, the imports resolved, the resources bound. Leaving it
until the port is finished would mean discovering all of that at the end, about a program
too big to bisect. So it runs now, against a program that is mostly stubs, and the stubs
shrink as the porting goes on.

WHAT COMES OUT. A genuine NE executable: Borland's Windows startup (C0WL), every ported
translation unit, the shipped data segments, a generated stub for every function not
ported yet, and the 42 shipped resources bound in. It is not playable and this tool does
not pretend otherwise -- `--report` says how much of it is still stubs.

HOW THE STUBS ARE FOUND. Not from a list that could drift: TLINK is run, its `Undefined
symbol` errors are read, a stub is generated for each, and the link is retried. The
remaining work is therefore whatever the linker says it is.

    tools/link.py               # -> build/link/COLONIZE.EXE
    tools/link.py --report      # the same, and what is still a stub
"""
import argparse, glob, json, os, re, struct, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import bcc  # noqa: E402

SRC_REPO = bcc.SRC_REPO
EXE = os.path.join(SRC_REPO, 'game', 'COLONIZE.EXE')
DOS = bcc.DOS
WORK = os.path.join(DOS, 'LINK')
OUT = os.path.join(REPO, 'build', 'link')
LIB = r'C:\BC452\LIB'
UNDEF = re.compile(r'Undefined symbol\s+_?(\w+)', re.I)


def header():
    """heap, stack and the module's own names, out of the shipped NE header."""
    b = open(EXE, 'rb').read()
    o = struct.unpack_from('<H', b, 0x3c)[0]
    g = lambda off: struct.unpack_from('<H', b, o + off)[0]          # noqa: E731
    return {'heap': g(0x10), 'stack': g(0x12), 'autodata': g(0x0e)}


def resources():
    """The shipped resources as a 16-bit .RES, read straight out of the image.

    The 1,902 game assets are not in here -- they are separate files. The NE carries 42
    numbered resources: cursors, icons, bitmaps, menus and one dialog.
    """
    b = open(EXE, 'rb').read()
    o = struct.unpack_from('<H', b, 0x3c)[0]
    rt = struct.unpack_from('<H', b, o + 0x24)[0]
    if not rt or rt == struct.unpack_from('<H', b, o + 0x26)[0]:
        return b'', 0
    p = o + rt
    shift = struct.unpack_from('<H', b, p)[0]
    p += 2
    out, n = bytearray(), 0
    while True:
        tid, count = struct.unpack_from('<HH', b, p)[0], 0
        if tid == 0:
            return bytes(out), n
        count = struct.unpack_from('<H', b, p + 2)[0]
        p += 8
        for _ in range(count):
            off, ln, flags, rid = struct.unpack_from('<HHHH', b, p)
            data = b[off << shift:(off << shift) + (ln << shift)]
            out += b'\xff' + struct.pack('<H', tid & 0x7fff)
            out += b'\xff' + struct.pack('<H', rid & 0x7fff)
            out += struct.pack('<HI', flags, len(data)) + data
            n += 1
            p += 12


def write_def(path, hdr):
    """The module definition. Heap and stack are the shipped program's own.

    No IMPORTS section: this is an ordinary Borland Windows link, so IMPORT.LIB resolves
    the API by name. The byte-exact repo's .DEF lists all 180 by ordinal because it
    relinks the shipped objects and has to reproduce the import table exactly; nothing
    here has to.
    """
    open(path, 'w', newline='\r\n').write('\n'.join([
        'NAME COLONIZE',
        "DESCRIPTION 'Sid Meier''s COLONIZATION -- win-port'",
        'EXETYPE WINDOWS',
        'CODE MOVEABLE DISCARDABLE',
        'DATA MOVEABLE MULTIPLE',
        'HEAPSIZE %d' % hdr['heap'],
        'STACKSIZE %d' % hdr['stack'],
    ]) + '\n')


def dosbox(lines, timeout=900):
    conf = os.path.join(DOS, 'link.conf')
    open(conf, 'w').write(
        '[dosbox]\nmemsize=32\n[cpu]\ncore=auto\ncputype=pentium\ncycles=max\n'
        '[autoexec]\nMOUNT C %s\nC:\nSET PATH=C:\\BC452\\BIN\nCD \\LINK\n%s\nEXIT\n'
        % (DOS, '\n'.join(lines)))
    try:
        subprocess.run(['dosbox-x', '-silent', '-nolog', '-conf', conf],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=timeout)
    except subprocess.TimeoutExpired:
        pass


def response(objs):
    """TLINK's response file. The object list is far past DOS's 127-character command
    line, and a newline ENDS a field -- so the comma-separated tail must sit on the same
    line as the last object."""
    rows = ['%s\\C0WL.OBJ+' % LIB]
    for i in range(0, len(objs), 6):
        chunk = '+'.join(objs[i:i + 6])
        rows.append(chunk + ('+' if i + 6 < len(objs) else
                             ',COLONIZE.EXE,COLONIZE.MAP,'
                             '%s\\CWL.LIB+%s\\MATHWL.LIB+%s\\IMPORT.LIB,COLONIZE.DEF'
                             % (LIB, LIB, LIB)))
    return '\n'.join(rows) + '\n'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report', action='store_true')
    ap.add_argument('--rounds', type=int, default=4)
    a = ap.parse_args()

    sources = sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c'))) + \
        [os.path.join(REPO, 'src', 'segdata.c'), os.path.join(REPO, 'src', 'winmain.c')]
    print('compiling %d translation units with Borland C++ 4.52' % len(sources))
    built = bcc.compile_all(sources)
    os.makedirs(WORK, exist_ok=True)
    objs, failed = [], []
    for i, src in enumerate(sources):
        obj, log = built[src]
        if obj is None or any(l.startswith('Error') for l in (log or '').splitlines()):
            failed.append((src, log))
            continue
        name = 'U%04d' % i
        open(os.path.join(WORK, name + '.OBJ'), 'wb').write(obj)
        objs.append(name)
    if failed:
        print('%d did not compile:' % len(failed))
        for src, log in failed[:5]:
            print('  %s' % os.path.relpath(src, REPO))
        return 1

    hdr = header()
    write_def(os.path.join(WORK, 'COLONIZE.DEF'), hdr)
    res, nres = resources()
    open(os.path.join(WORK, 'COLONIZE.RES'), 'wb').write(res)

    stubs, exe = set(), os.path.join(WORK, 'COLONIZE.EXE')
    for rnd in range(a.rounds):
        if stubs:
            body = ['/* GENERATED by tools/link.py -- every function the linker could not',
                    ' * find. Each one is a reconstruction not ported yet; this file is the',
                    ' * remaining work, written by the linker rather than by a list.',
                    ' */']
            body += ['void far %s(void) { }' % s for s in sorted(stubs)]
            open(os.path.join(WORK, 'STUBS.C'), 'w').write('\n'.join(body) + '\n')
            obj, log = bcc.compile_unit(os.path.join(WORK, 'STUBS.C'))
            if obj is None:
                print('the stub file did not compile:\n' + (log or '')[:800])
                return 1
            open(os.path.join(WORK, 'STUBS.OBJ'), 'wb').write(obj)
        names = objs + (['STUBS'] if stubs else [])
        open(os.path.join(WORK, 'LINK.RSP'), 'w', newline='\r\n').write(response(names))
        for f in ('COLONIZE.EXE', 'TL.TXT'):
            try:
                os.remove(os.path.join(WORK, f))
            except FileNotFoundError:
                pass
        dosbox(['TLINK /Twe /c /x @LINK.RSP > TL.TXT'])
        log = ''
        if os.path.exists(os.path.join(WORK, 'TL.TXT')):
            log = open(os.path.join(WORK, 'TL.TXT'), errors='replace').read()
        missing = set(UNDEF.findall(log))
        if os.path.exists(exe) and not missing:
            break
        if not missing:
            print('link failed and named no missing symbol:\n' +
                  '\n'.join(log.splitlines()[:20]))
            return 1
        print('round %d: %d undefined symbols' % (rnd + 1, len(missing)))
        stubs |= missing
    else:
        print('still unresolved after %d rounds' % a.rounds)
        return 1

    dosbox(['BRC COLONIZE.RES COLONIZE.EXE > BRC.TXT'])
    b = open(exe, 'rb').read()
    o = struct.unpack_from('<H', b, 0x3c)[0] if b[:2] == b'MZ' else 0
    ne = o and b[o:o + 2] == b'NE'
    os.makedirs(OUT, exist_ok=True)
    open(os.path.join(OUT, 'COLONIZE.EXE'), 'wb').write(b)
    print('linked: %s, %d bytes%s' % ('NE' if ne else 'NOT an NE', len(b),
          ', %d segments, autodata %d, CS:IP %d:%04x'
          % (struct.unpack_from('<H', b, o + 0x1c)[0],
             struct.unpack_from('<H', b, o + 0x0e)[0],
             struct.unpack_from('<H', b, o + 0x16)[0],
             struct.unpack_from('<H', b, o + 0x14)[0]) if ne else ''))
    print('resources: %d bound' % nres)
    print('%d translation units, %d stubbed functions still to port'
          % (len(objs), len(stubs)))
    json.dump({'what': 'what the linker could not find: the remaining work, its own list',
               'translation_units': len(objs), 'stubs': sorted(stubs)},
              open(os.path.join(REPO, 'model', 'stubs.json'), 'w'), indent=1)
    if a.report:
        for s in sorted(stubs)[:40]:
            print('   ' + s)
    return 0 if ne else 1


if __name__ == '__main__':
    sys.exit(main())
