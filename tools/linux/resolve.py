#!/usr/bin/env python3
"""The Linux build's crash report as functions and lines (roadmap W8).

    python3 tools/linux/resolve.py [build/run-linux/port.log]

A Winelib program is an ELF library Wine loads anywhere, so the report carries an anchor --
where w16_crash_install was loaded (src/host/w16_crash.c); each address less the anchor plus
nm's address for it is an address addr2line knows. Runs nm and addr2line in the Linux machine
($PORT_LINUX_MACHINE, default colw8)."""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SO = os.path.join(ROOT, "build", "linux", "colonize.exe.so")
M = ["orb", "-m", os.environ.get("PORT_LINUX_MACHINE", "colw8"), "-w", ROOT]

text = open(sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "build", "run-linux", "port.log")).read()
anchor = int(re.search(r"^crash anchor ([0-9a-f]+)", text, re.M).group(1), 16)
nm = subprocess.run(M + ["nm", SO], capture_output=True, text=True).stdout
base = int(re.search(r"^([0-9a-f]+) T w16_crash_install$", nm, re.M).group(1), 16)
addrs = [(tag, int(a, 16)) for tag, a in re.findall(r"^(crash at|frame|entered) ([0-9a-f]+)", text, re.M) if int(a, 16)]
out = subprocess.run(M + ["addr2line", "-f", "-C", "-s", "-e", SO] + ["%#x" % (a - anchor + base) for _t, a in addrs],
                     capture_output=True, text=True).stdout.splitlines()
for (tag, a), fn, line in zip(addrs, out[0::2], out[1::2]):
    print("%-9s %08x  %-40s %s" % (tag, a, fn, line))
