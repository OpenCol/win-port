#!/usr/bin/env python3
"""Compile one translation unit with the ORIGINAL Borland C++ 4.52, targeting Win16.

BCC.EXE is a 16-bit DPMI program, so it runs under DOSBox-X rather than natively or under
Wine. This mounts a tree of its OWN making as C: -- the compiler is symlinked in from the
byte-exact repo, which owns it, and nothing here ever writes there.

    tools/bcc.py src/foo.c              # -> build/obj/FOO.OBJ, or the compiler's errors
    tools/bcc.py --flags '-ml -WS' ...

Set COL_BC452 to point somewhere else for the compiler, and COL_SRC_REPO for the repo it
comes from. Neither is copied into this tree: the toolchain is 40 MB of someone else's
installer output and has one authoritative copy.
"""
import argparse, os, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC_REPO = os.environ.get('COL_SRC_REPO',
                          os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
BC452 = os.environ.get('COL_BC452', os.path.join(SRC_REPO, 'analysis', 'toolchain', 'BC452'))
DOS = os.path.join(REPO, 'build', 'dos')
WORK = os.path.join(DOS, 'WORK')
# -ml  large model: the shipped program is large model and the map code needs far data.
# -WS  a Windows smart-callback application: the target is Windows 3.1.
# -Ot  optimise for speed, as the shipped build did (docs/findings/toolchain.md there).
# -3   80386 instructions, likewise.
# -dc  string literals into the code segment. Not for compatibility -- for room. DGROUP
#      is 64 KiB and already holds the shipped automatic data segment (22,620 bytes), the
#      heap and the stack, leaving about 6.7 KiB for everything the remaining 3,045
#      functions declare. Literals are the part of that which does not have to be there.
DEFAULT_FLAGS = '-ml -WS -Ot -3 -dc'


def link(dst, src):
    if os.path.islink(dst) or os.path.exists(dst):
        if os.path.realpath(dst) == os.path.realpath(src):
            return
        os.remove(dst) if os.path.islink(dst) else shutil.rmtree(dst)
    os.symlink(src, dst)


def compile_unit(path, flags=DEFAULT_FLAGS, timeout=240, tdump=False):
    """(object bytes or None, the compiler's own log), or with `tdump` a third value:
    TDUMP's listing of the object, which is how the emulator-side differential finds a
    function's bytes and its fixups inside a multi-symbol object."""
    if not os.path.isdir(BC452):
        return None, 'no Borland C++ 4.52 at %s (set COL_BC452)' % BC452
    os.makedirs(WORK, exist_ok=True)
    link(os.path.join(DOS, 'BC452'), BC452)
    link(os.path.join(DOS, 'INC'), os.path.join(REPO, 'include'))
    stem = os.path.splitext(os.path.basename(path))[0][:8].upper()
    for f in ('T.C', stem + '.OBJ', 'CC.TXT', 'TD.TXT'):
        try:
            os.remove(os.path.join(WORK, f))
        except FileNotFoundError:
            pass
    shutil.copyfile(path, os.path.join(WORK, 'T.C'))
    conf = os.path.join(DOS, 'bcc.conf')
    open(conf, 'w').write(
        '[dosbox]\nmemsize=32\n[cpu]\ncore=auto\ncputype=pentium\ncycles=max\n'
        '[autoexec]\n'
        'MOUNT C %s\nC:\nSET PATH=C:\\BC452\\BIN\nCD \\WORK\n'
        'BCC -c %s -IC:\\BC452\\INCLUDE;C:\\INC -o%s.OBJ T.C > CC.TXT\n%sEXIT\n'
        % (DOS, flags, stem,
           'TDUMP %s.OBJ > TD.TXT\n' % stem if tdump else ''))
    try:
        subprocess.run(['dosbox-x', '-silent', '-nolog', '-conf', conf],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=timeout)
    except subprocess.TimeoutExpired:
        return None, 'dosbox-x timed out after %ds' % timeout
    log = ''
    p = os.path.join(WORK, 'CC.TXT')
    if os.path.exists(p):
        log = open(p, errors='replace').read()
    obj = os.path.join(WORK, stem + '.OBJ')
    td = ''
    tdp = os.path.join(WORK, 'TD.TXT')
    if tdump and os.path.exists(tdp):
        td = open(tdp, errors='replace').read()
    if not os.path.exists(obj) or os.path.getsize(obj) == 0:
        return (None, log or 'BCC produced no object and said nothing') + \
            ((td,) if tdump else ())
    data = open(obj, 'rb').read()
    return (data, log) + ((td,) if tdump else ())


CHUNK = 32          # DOSBox's [autoexec] stops running somewhere past this many lines:
                    # 20 files compile, 132 silently produce nothing at all.


def compile_all(paths, flags=DEFAULT_FLAGS, timeout=1200):
    """Compile many units in ONE DOSBox session.

    Booting DOSBox-X costs a few seconds and compiling one of these files costs a
    fraction of one, so a file-at-a-time loop spends all its time on the emulator. This
    writes one autoexec that compiles the lot and reads the logs afterwards.

    Returns {path: (object bytes or None, that file's log)}.
    """
    if not os.path.isdir(BC452):
        return {p: (None, 'no Borland C++ 4.52 at %s' % BC452) for p in paths}
    if len(paths) > CHUNK:
        out = {}
        for i in range(0, len(paths), CHUNK):
            out.update(compile_all(paths[i:i + CHUNK], flags, timeout))
        return out
    os.makedirs(WORK, exist_ok=True)
    link(os.path.join(DOS, 'BC452'), BC452)
    link(os.path.join(DOS, 'INC'), os.path.join(REPO, 'include'))
    stems, lines = {}, []
    for i, p in enumerate(paths):
        stem = 'U%04d' % i
        stems[p] = stem
        shutil.copyfile(p, os.path.join(WORK, stem + '.C'))
        for f in (stem + '.OBJ', stem + '.TXT'):
            try:
                os.remove(os.path.join(WORK, f))
            except FileNotFoundError:
                pass
        lines.append('BCC -c %s -IC:\\BC452\\INCLUDE;C:\\INC -o%s.OBJ %s.C > %s.TXT'
                     % (flags, stem, stem, stem))
    conf = os.path.join(DOS, 'bccall.conf')
    open(conf, 'w').write(
        '[dosbox]\nmemsize=32\n[cpu]\ncore=auto\ncputype=pentium\ncycles=max\n'
        '[autoexec]\nMOUNT C %s\nC:\nSET PATH=C:\\BC452\\BIN\nCD \\WORK\n%s\nEXIT\n'
        % (DOS, '\n'.join(lines)))
    try:
        subprocess.run(['dosbox-x', '-silent', '-nolog', '-conf', conf],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=timeout)
    except subprocess.TimeoutExpired:
        pass
    out = {}
    for p, stem in stems.items():
        log = ''
        lp = os.path.join(WORK, stem + '.TXT')
        if os.path.exists(lp):
            log = open(lp, errors='replace').read()
        op = os.path.join(WORK, stem + '.OBJ')
        obj = open(op, 'rb').read() if os.path.exists(op) and os.path.getsize(op) else None
        out[p] = (obj, log)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('source', nargs='*')
    ap.add_argument('--flags', default=DEFAULT_FLAGS)
    ap.add_argument('--out')
    a = ap.parse_args()
    if len(a.source) != 1:
        results = compile_all(a.source, a.flags)
        bad = 0
        for p in a.source:
            obj, log = results[p]
            errs = [ln for ln in (log or '').splitlines() if ln.startswith('Error')]
            if obj is None or errs:
                bad += 1
                print('%-40s FAILED' % os.path.relpath(p, REPO))
                for e in errs[:4]:
                    print('    ' + e)
        print('%d of %d compiled with Borland C++ 4.52'
              % (len(a.source) - bad, len(a.source)))
        return 1 if bad else 0
    obj, log = compile_unit(a.source[0], a.flags)
    sys.stdout.write(log or '')
    if obj is None:
        print('COMPILE FAILED (%s)' % a.flags, file=sys.stderr)
        return 1
    out = a.out or os.path.join(REPO, 'build', 'obj',
                                os.path.splitext(os.path.basename(a.source[0]))[0] + '.obj')
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, 'wb').write(obj)
    print('%d bytes -> %s' % (len(obj), os.path.relpath(out, REPO)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
