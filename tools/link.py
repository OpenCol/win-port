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
import shutil  # noqa: E402
import bcc  # noqa: E402

SRC_REPO = bcc.SRC_REPO
EXE = os.path.join(SRC_REPO, 'game', 'COLONIZE.EXE')
DOS = bcc.DOS
WORK = os.path.join(DOS, 'LINK')
OUT = os.path.join(REPO, 'build', 'link')
LIB = r'C:\BC452\LIB'
# The leading underscore is KEPT: it is what tells a C symbol from a `pascal` one.
# TLINK reports a pascal reference as `Undefined symbol PORT_WNDPROC` -- uppercase, no
# underscore -- and a stub written `void far PORT_WNDPROC(void) {}` publishes
# `_PORT_WNDPROC`, which does not satisfy it. The linker then reported the same symbol
# every round and the loop ran to its limit without converging.
UNDEF = re.compile(r'Undefined symbol\s+(_?)(\w+)', re.I)


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
        # 32 MB was enough for a thousand objects and not for two thousand: TLINK ran out
        # under DPMI and said only `Unexpected error 8003`, naming no symbol, which
        # reads like a broken object rather than a full heap.
        '[dosbox]\nmemsize=64\n[cpu]\ncore=auto\ncputype=pentium\ncycles=max\n'
        # Borland's DPMI loader takes its ceiling from DPMIMEM. Without it TLINK stops
        # between 2,100 and 2,200 objects with `Unexpected error 8003` and no symbol
        # named -- which reads like a broken object rather than a full heap.
        '[autoexec]\nMOUNT C %s\nC:\nSET PATH=C:\\BC452\\BIN\n'
        'SET DPMIMEM=MAXMEM 60000\nCD \\LINK\n%s\nEXIT\n'
        % (DOS, '\n'.join(lines)))
    try:
        subprocess.run(['dosbox-x', '-silent', '-nolog', '-conf', conf],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=timeout)
    except subprocess.TimeoutExpired:
        pass


# A LIBRARY was tried here and taken out again. TLINK pulls a library module only when
# it resolves a reference, so COLPORT.LIB gave a 197 KB executable with seven segments
# and nothing to stub -- and "0 stubbed functions still to port" is this project's
# measure of what is left, not a result. The object COUNT is what has to come down, so
# several ports share a translation unit instead: see group_sources().


DECL = re.compile(r'^\s*(?:extern\s+)?((?:[A-Za-z_]\w*\s+|\*)+)(\w+)\s*\(([^;{]*)\)\s*;',
                  re.M)


def declares(text):
    """{name: its declared signature} for every prototype in a ported unit."""
    out = {}
    for m in DECL.finditer(text):
        ret, name, args = m.group(1), m.group(2), m.group(3)
        out[name] = ' '.join((ret + '|' + args).split())
    return out


def group_sources(paths, per=5):
    """Concatenate ported units into fewer translation units, for the LINK only.

    TLINK holds every object named on its command line at once and runs out somewhere
    between 2,100 and 2,200 of them, saying only `Unexpected error 8003`; raising
    DPMIMEM and DOSBox's memsize changes nothing, so the object COUNT is what has to come
    down. One function per file is how this repo is written and how both harnesses read
    it, and nothing about that has to be true of the object the linker sees.

    Three things do not survive a naive concatenation. `#define NOGDI` has to precede the
    first windows.h, so a unit needing it is only grouped with others that do. The
    COL_EXTERN_C block is per file, so copies after the first come out. And two ports can
    declare the SAME callee with different prototypes -- `window_relayout` is spelt two
    ways in the corpus -- which no per-file build would ever notice; those two are simply
    kept apart, and the disagreement is reported rather than papered over.
    """
    import io
    text_of = {x: open(x).read() for x in paths}
    decl_of = {x: declares(text_of[x]) for x in paths}
    groups, clashes = [], {}
    for want_nogdi in (False, True):
        pool = [x for x in paths
                if ('#define NOGDI' in text_of[x]) == want_nogdi]
        open_groups = []          # [(members, merged declarations)]
        for x in pool:
            for g in open_groups:
                if len(g[0]) >= per:
                    continue
                bad = [n for n, sig in decl_of[x].items()
                       if n in g[1] and g[1][n] != sig]
                if bad:
                    clashes.setdefault(bad[0], set()).add(os.path.basename(x))
                    continue
                g[0].append(x)
                g[1].update(decl_of[x])
                break
            else:
                open_groups.append(([x], dict(decl_of[x])))
        groups += [g[0] for g in open_groups]
    out = []
    for g in groups:
        buf, seen_extern_c = io.StringIO(), False
        if any('#define NOGDI' in text_of[x] for x in g):
            buf.write('#define NOGDI\n')
        for x in g:
            text = text_of[x].replace('#define NOGDI\n', '')
            if seen_extern_c:
                text = re.sub(r'#ifdef __cplusplus\n#define COL_EXTERN_C[^\n]*\n'
                              r'#else\n#define COL_EXTERN_C\n#endif\n', '', text)
            elif 'COL_EXTERN_C' in text:
                seen_extern_c = True
            buf.write('\n/* ---- %s ---- */\n' % os.path.basename(x))
            buf.write(text)
        out.append((g, buf.getvalue()))
    return out, clashes


def response(objs, lib=None):
    """TLINK's response file. The object list is far past DOS's 127-character command
    line, and a newline ENDS a field -- so the comma-separated tail must sit on the same
    line as the last object."""
    libs = '%s\\CWL.LIB+%s\\MATHWL.LIB+%s\\IMPORT.LIB' % (LIB, LIB, LIB)
    if lib:
        libs = lib + '+' + libs
    tail = (',COLONIZE.EXE,COLONIZE.MAP,' + libs + ',COLONIZE.DEF')
    if not objs:
        return '%s\\C0WL.OBJ%s\n' % (LIB, tail)
    rows = ['%s\\C0WL.OBJ+' % LIB]
    for i in range(0, len(objs), 6):
        chunk = '+'.join(objs[i:i + 6])
        rows.append(chunk + ('+' if i + 6 < len(objs) else tail))
    return '\n'.join(rows) + '\n'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report', action='store_true')
    ap.add_argument('--rounds', type=int, default=4)
    ap.add_argument('--group', type=int, default=5,
                    help='how many ported units share a translation unit: '
                         '1 is one per file, which TLINK cannot take past ~2,150')
    ap.add_argument('--limit', type=int,
                    help='link only the first N units: for finding where TLINK stops')
    a = ap.parse_args()

    ported = sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c')))
    apart = [os.path.join(REPO, 'src', 'segdata.c'),
             os.path.join(REPO, 'src', 'winmain.c')]
    os.makedirs(WORK, exist_ok=True)
    groups, clashes = group_sources(ported, a.group)
    if clashes:
        # Two ports declaring one callee differently. A per-file build never compares
        # them, so this is the first time the corpus has been asked to agree with itself.
        print('%d callees are declared two ways across the corpus, kept apart here:'
              % len(clashes))
        for n, who in sorted(clashes.items())[:8]:
            print('   %-32s %s' % (n, ', '.join(sorted(who)[:3])))
    print('%d ported units in %d translation units, with Borland C++ 4.52'
          % (len(ported), len(groups) + len(apart)))
    stage = os.path.join(WORK, 'GRP')
    if os.path.isdir(stage):
        shutil.rmtree(stage)
    os.makedirs(stage)
    sources, of_group = [], {}
    for gi, (members, text) in enumerate(groups):
        f = os.path.join(stage, 'G%04d.C' % gi)
        open(f, 'w').write(text)
        sources.append(f)
        of_group[f] = members
    sources += apart
    built = bcc.compile_all(sources)
    # A group that does not compile is SPLIT, not lost. The packing heuristic reads
    # prototypes with a regex and will always miss some way two ports can disagree;
    # falling back to one unit per file for exactly those groups keeps the build correct
    # however good the heuristic is, and still leaves the object count far under TLINK's.
    def broke(src):
        obj, log = built[src]
        return obj is None or any(l.startswith('Error')
                                  for l in (log or '').splitlines())

    regroup = [m for f in sources if f in of_group and broke(f)
               for m in of_group[f]]
    if regroup:
        print('%d groups did not compile; their %d units go one to a unit'
              % (len({f for f in sources if f in of_group and broke(f)}),
                 len(regroup)))
        sources = [f for f in sources if not (f in of_group and broke(f))]
        for x in regroup:
            f = os.path.join(stage, 'S%04d.C' % len(of_group))
            shutil.copyfile(x, f)
            of_group[f] = [x]
            sources.append(f)
        built.update(bcc.compile_all([f for f in sources if f not in built]))
    objs, direct, failed = [], [], []
    for i, src in enumerate(sources):
        obj, log = built[src]
        if obj is None or any(l.startswith('Error') for l in (log or '').splitlines()):
            # Name the ported files in the group, not the generated unit: the group is
            # this tool's doing and nobody can go and read G0031.C for an answer.
            for m in of_group.get(src, [src]):
                failed.append((m, log))
            continue
        name = 'U%04d' % i
        open(os.path.join(WORK, name + '.OBJ'), 'wb').write(obj)
        (direct if os.path.basename(src).lower() in ('segdata.c', 'winmain.c')
         else objs).append(name)
    if a.limit:
        objs = objs[:a.limit]
        print('linking only the first %d of them' % len(objs))
    if failed:
        print('%d did not compile:' % len(failed))
        for src, log in failed[:5]:
            print('  %s' % os.path.relpath(src, REPO))
        return 1

    # Clear the PUBLISHED executable before anything else. A failed link left the last
    # successful one in build/link/, and tests/boot.py then booted a day-old binary and
    # reported "the loader accepted the image, WinMain returned and the runtime exited"
    # while the link that run had just failed. A stale green is worse than a red.
    try:
        os.remove(os.path.join(OUT, 'COLONIZE.EXE'))
    except (FileNotFoundError, NotADirectoryError):
        pass

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
            body += ['void %s%s(void) { }' % ('far ' if u else 'pascal ', n)
                     for u, n in sorted(stubs)]
            open(os.path.join(WORK, 'STUBS.C'), 'w').write('\n'.join(body) + '\n')
            obj, log = bcc.compile_unit(os.path.join(WORK, 'STUBS.C'))
            if obj is None:
                print('the stub file did not compile:\n' + (log or '')[:800])
                return 1
            open(os.path.join(WORK, 'STUBS.OBJ'), 'wb').write(obj)
        names = objs + direct + (['STUBS'] if stubs else [])
        open(os.path.join(WORK, 'LINK.RSP'), 'w', newline='\r\n').write(
            response(names))
        for f in ('COLONIZE.EXE', 'TL.TXT'):
            try:
                os.remove(os.path.join(WORK, f))
            except FileNotFoundError:
                pass
        dosbox(['TLINK /Twe /c /x @LINK.RSP > TL.TXT'])
        log = ''
        if os.path.exists(os.path.join(WORK, 'TL.TXT')):
            log = open(os.path.join(WORK, 'TL.TXT'), errors='replace').read()
        missing = {(u, n) for u, n in UNDEF.findall(log)}
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
    print('%d ported units in %d translation units, %d stubbed functions still to port'
          % (len(ported), len(objs) + len(direct), len(stubs)))
    json.dump({'what': 'what the linker could not find: the remaining work, its own list',
               'ported_units': len(ported), 'translation_units': len(objs) + len(direct),
               'stubs': sorted(n for _u, n in stubs)},
              open(os.path.join(REPO, 'model', 'stubs.json'), 'w'), indent=1)
    if a.report:
        for _u, s in sorted(stubs)[:40]:
            print('   ' + s)
    return 0 if ne else 1


if __name__ == '__main__':
    sys.exit(main())
