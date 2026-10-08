#!/usr/bin/env python3
"""Roadmap W9 step 1: every pointer<->integer cast a 64-bit build would truncate.

    python3 tools/cast_audit.py [--toolchain mingw-x64] [--list] [--record]

Compiles every source the program is linked from (src/game, gfx, platform, host, gen, asm) for
x86_64 with -Wpointer-to-int-cast -Wint-to-pointer-cast and counts the two warnings, by file and
by kind. The gate: zero. --record writes status/casts.json (tools/status.py reads it).
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
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402

# C warns; C++ refuses the same cast to a narrower integer ("loses precision", -fpermissive)
KINDS = re.compile(r"(?:warning|error): .*(?:\[-W(pointer-to-int-cast|int-to-pointer-cast)\]|(loses precision))")


def sources():
    out = list(ca.sources())
    for d in ("host", "gen", "asm"):
        out += sorted(p for p in glob.glob(os.path.join(ROOT, "src", d, "*.c")) if not p.endswith("selftest.c"))
    return out


def one(tc, path):
    cc, cxx, extra = ca.TOOLCHAINS[tc]
    cpp = ca.is_cpp(path)
    cmd = ([cxx, "-x", "c++", "-std=gnu++03"] if cpp else [cc, "-x", "c", "-std=gnu99"]) + extra + [
        "-fsyntax-only", "-Wpointer-to-int-cast", "-Wint-to-pointer-cast",   # not -w: it silences these too
        "-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include"), path]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", stdin=subprocess.DEVNULL, timeout=300)
    hits = [(l, KINDS.search(l).group(1) or "pointer-to-int-cast") for l in r.stderr.splitlines() if KINDS.search(l)]
    return os.path.relpath(path, ROOT), r.returncode == 0, hits, r.stderr


def main(argv):
    tc = argv[argv.index("--toolchain") + 1] if "--toolchain" in argv else "mingw-x64"
    srcs = sources()
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        res = list(ex.map(lambda p: one(tc, p), srcs))
    kinds = collections.Counter(k for _p, _ok, hits, _e in res for _l, k in hits)
    files = {p: len(hits) for p, _ok, hits, _e in res if hits}
    failed = [(p, e) for p, ok, _h, e in res if not ok]
    total = sum(kinds.values())
    print("%s: %d of %d files compile; %d pointer<->integer cast warnings in %d files (%s)" % (
        tc, len(srcs) - len(failed), len(srcs), total, len(files),
        ", ".join("%s %d" % kv for kv in sorted(kinds.items())) or "none"))
    for p, e in failed[:5]:
        print("  does not compile: %s: %s" % (p, next((l for l in e.splitlines() if "error" in l), "")[:140]))
    if "--list" in argv:
        for p, _ok, hits, _e in res:
            for l, _k in hits:
                print("  " + l.replace(ROOT + "/", "")[:200])
    if "--record" in argv:
        with open(os.path.join(ROOT, "status", "casts.json"), "w") as f:
            json.dump({"toolchain": tc, "sources": len(srcs), "compile": len(srcs) - len(failed), "warnings": total,
                       "kinds": dict(kinds), "files": files}, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if total == 0 and not failed else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
