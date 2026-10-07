#!/usr/bin/env python3
"""W1's gate: compile every ported file, alone, and count how many compile.

    python3 tools/compile_all.py                       # MinGW i686 (the default toolchain)
    python3 tools/compile_all.py --toolchain winegcc   # Winelib, 32-bit (W8's target, W1's gate)
    python3 tools/compile_all.py --toolchain host      # the machine's own cc/c++, for a quick look
    python3 tools/compile_all.py --only src/game/add_gold.c -v    # one file, its whole log
    python3 tools/compile_all.py --record              # also write status/compile-<toolchain>.json

Each file in src/game, src/gfx and src/platform is compiled with -fsyntax-only and -Wall: as
C++ when its `flags:` line carries Borland's -P (436 reconstructions are C++), else as C.
Include order: include/gen (this repository's generated headers, which win), then include
(the imported ones). A file "compiles" when the compiler exits 0; warnings are counted, not
failed. Failures are grouped by their first error, largest group first, because one header
fix usually clears a whole group.

Gate (roadmap W1): N of N, zero errors, on MinGW i686 and on winegcc.
"""
import collections
import concurrent.futures
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LAYERS = ("game", "gfx", "platform")
TOOLCHAINS = {
    "mingw-i686": ("i686-w64-mingw32-gcc", "i686-w64-mingw32-g++", []),
    "winegcc": ("winegcc", "wineg++", ["-m32"]),
    "host": ("cc", "c++", []),
}
FLAGS = re.compile(r"^\s*\*?\s*flags:\s*(.+?)\s*$", re.M)
ERROR = re.compile(r"(?:error|fatal error): (.*)$")


def sources():
    return sorted(p for l in LAYERS for p in glob.glob(os.path.join(ROOT, "src", l, "*.c")))


def is_cpp(path):
    m = FLAGS.search(open(path, errors="replace").read(4000))
    return bool(m and re.search(r"(?<!\S)-P(?!\S)", m.group(1)))


def compile_one(tc, path):
    cc, cxx, extra = TOOLCHAINS[tc]
    cpp = is_cpp(path)
    cmd = ([cxx, "-x", "c++", "-std=gnu++03"] if cpp else [cc, "-x", "c", "-std=gnu99"]) + extra + [
        "-fsyntax-only", "-Wall", "-I", os.path.join(ROOT, "include", "gen"),
        "-I", os.path.join(ROOT, "include"), path]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    first = None
    for line in r.stderr.splitlines():
        m = ERROR.search(line)
        if m:
            # names and numbers vary per file; the shape of the message groups them
            first = re.sub(r"'[^']*'", "'X'", m.group(1)).strip()
            first_raw = m.group(1).strip()
            break
    warnings = sum(1 for l in r.stderr.splitlines() if ": warning:" in l)
    return {"path": os.path.relpath(path, ROOT), "ok": r.returncode == 0, "cpp": cpp,
            "first": first, "first_raw": first_raw if first else None,
            "warnings": warnings, "log": r.stderr}


def main(argv):
    def opt(name, default=None):
        return argv[argv.index(name) + 1] if name in argv else default
    tc = opt("--toolchain", "mingw-i686")
    if tc not in TOOLCHAINS:
        sys.exit("toolchain: one of %s" % ", ".join(TOOLCHAINS))
    files = [os.path.join(ROOT, opt("--only"))] if opt("--only") else sources()
    jobs = int(opt("-j", str(os.cpu_count() or 4)))
    with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
        res = list(ex.map(lambda p: compile_one(tc, p), files))
    ok = [r for r in res if r["ok"]]
    bad = [r for r in res if not r["ok"]]
    groups = collections.defaultdict(list)
    for r in bad:
        groups[r["first"] or "(failed without an error line)"].append(r)
    order = sorted(groups.items(), key=lambda kv: (-len(kv[1]), kv[0]))
    if "-v" in argv:
        for r in res:
            print(r["log"])
    print("compiles: %d of %d with %s (%d C++); %d warnings" % (
        len(ok), len(res), tc, sum(1 for r in res if r["cpp"]), sum(r["warnings"] for r in res)))
    print("distinct first errors: %d" % len(groups))
    for msg, rs in order[:int(opt("--errors", "15"))]:
        print("  %5d  %-70s e.g. %s: %s" % (len(rs), msg[:70], os.path.basename(rs[0]["path"]),
                                           (rs[0]["first_raw"] or "")[:60]))
    if "--record" in argv and not opt("--only"):
        os.makedirs(os.path.join(ROOT, "status"), exist_ok=True)
        cc = TOOLCHAINS[tc][0]
        ver = subprocess.run([cc, "--version"], capture_output=True, text=True, errors="replace").stdout
        out = {"toolchain": tc, "compiler": (ver.splitlines() or [cc])[0].strip(), "total": len(res), "ok": len(ok),
               "first_errors": [[len(rs), msg] for msg, rs in order[:20]]}
        p = os.path.join(ROOT, "status", "compile-%s.json" % tc)
        with open(p, "w") as f:
            json.dump(out, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if not bad else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
