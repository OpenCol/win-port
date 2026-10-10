#!/usr/bin/env python3
"""Read a player's crash log (roadmap W10): every address in colonize.exe, named.

    python3 tools/crash_log.py colonize-port.log [colonize.exe]

The log is what the port writes beside the kit (src/host/w16_crash.c): `crash at 004eb0e5
(colonize.exe+0xeb0e5)` and the like. The program is the one that ran -- the kit's bin/colonize.exe
of that release (gh release download), or build/colonize.exe when the log's link stamp matches.
The kit's program has no debug information but keeps its symbols: an address is named by the
nearest symbol below it, and by file and line when the program has them. The image base is
fixed (--disable-dynamicbase), so the module offset alone places the address.
"""
import bisect
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        sys.exit(__doc__)
    text = open(argv[0], errors="replace").read()
    x64 = " x64 build" in text
    exe = argv[1] if len(argv) > 1 else os.path.join(ROOT, "build", "x64" if x64 else "", "colonize.exe")
    base = 0x140000000 if x64 else 0x400000
    prefix = "x86_64-w64-mingw32-" if x64 else "i686-w64-mingw32-"
    syms = []
    for l in subprocess.run([prefix + "nm", "-C", exe], capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]+) [tT] (.*)", l)
        if m:
            syms.append((int(m.group(1), 16), m.group(2)))
    syms.sort()
    keys = [a for a, _ in syms]
    offs = sorted({int(m, 16) for m in re.findall(r"\(colonize\.exe\+0x([0-9a-f]+)\)", text, re.I)})
    lines = {}
    if offs:
        out = subprocess.run([prefix + "addr2line", "-e", exe] + ["%x" % (base + o) for o in offs],
                             capture_output=True, text=True).stdout.splitlines()
        lines = {o: l for o, l in zip(offs, out) if not l.startswith("??")}
    names = {}
    for o in offs:
        i = bisect.bisect_right(keys, base + o) - 1
        names[o] = "%s+%#x" % (syms[i][1], base + o - syms[i][0]) if i >= 0 else "?"
        if o in lines:
            names[o] += "  " + os.path.relpath(lines[o], ROOT) if lines[o].startswith(ROOT) else "  " + lines[o]
    for l in text.splitlines():
        m = re.search(r"\(colonize\.exe\+0x([0-9a-f]+)\)", l, re.I)
        print(l + ("    " + names[int(m.group(1), 16)] if m else ""))


if __name__ == "__main__":
    main(sys.argv[1:])
