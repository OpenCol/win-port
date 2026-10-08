#!/usr/bin/env python3
"""Evaluate src/gen/alias_paths.txt for a target (roadmap W9).

    python3 tools/alias_offsets.py [--toolchain mingw-i686|mingw-x64]   the offsets; on i686 also
                                                                        the check: each equals the game's

Compiles one translation unit -- create_game_globals.c, where MainWin is defined whole, and an
array per global sized by its offset expression plus one -- for the target, and reads the sizes
back from nm. tools/undefined.py uses this for a 64-bit target's --defsym; for i386 the game's
offsets stand, and this checks that the expressions agree with them.
"""
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402

PATHS = os.path.join(ROOT, "src", "gen", "alias_paths.txt")
NM = {"mingw-i686": "i686-w64-mingw32-nm", "mingw-x64": "x86_64-w64-mingw32-nm"}


def paths():
    out = []
    for line in open(PATHS):
        if line.startswith("#") or not line.strip():
            continue
        n, c, off, expr = line.rstrip("\n").split(" ", 3)
        out.append((n, c, int(off, 16), expr))
    return out


def offsets(toolchain, extra=()):
    """{name: (container, offset)} as the target lays MainWin out. Each expression is read back from
    the compiler's own message about it -- `char (*)[N]` added to a double is an error naming N --
    which needs no linker and works for COFF objects too (their nm has no sizes)"""
    _cc, cxx, flags = ca.TOOLCHAINS[toolchain]
    ps = paths()
    head = '#include "%s"' % os.path.join(ROOT, "src", "game", "create_game_globals.c")
    src = [head] + ["int port_alias_off_%s = (char (*)[%s + 1])0 + 0.5;" % (n, e) for n, _c, _o, e in ps]
    with tempfile.TemporaryDirectory() as d:
        cpp = os.path.join(d, "alias_offsets.cpp")
        open(cpp, "w").write("\n".join(src) + "\n")
        r = subprocess.run([cxx, "-x", "c++", "-std=gnu++03", "-fsyntax-only", "-w", "-fmax-errors=0"] + list(flags)
                           + list(extra) + ["-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include"),
                                            "-I", os.path.join(ROOT, "src", "game"), cpp], capture_output=True, text=True)
    got = {}
    for m in re.finditer(r"alias_offsets\.cpp:(\d+):\d+: error: invalid operands of types 'char \(\*\)\[(\d+)\]'", r.stderr):
        got[int(m.group(1))] = int(m.group(2)) - 1
    if len(got) != len(ps):
        sys.exit("alias offsets: %d of %d expressions evaluated:\n%s" % (len(got), len(ps), r.stderr[-2000:]))
    return {n: (c, got[i + 2]) for i, (n, c, _o, _e) in enumerate(ps)}


def main(argv):
    tc = argv[argv.index("--toolchain") + 1] if "--toolchain" in argv else "mingw-i686"
    got = offsets(tc)
    bad = 0
    for n, c, off, _e in paths():
        mark = ""
        if "64" not in tc and got[n][1] != off:
            mark, bad = "  DIFFERS from the game's %#x" % off, bad + 1
        print("%-24s %s+%#x%s" % (n, c, got[n][1], mark))
    if "64" not in tc:
        print("%d of %d agree with the game's offsets" % (len(got) - bad, len(got)))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
