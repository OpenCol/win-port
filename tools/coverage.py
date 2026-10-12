#!/usr/bin/env python3
"""Which of the game's functions the tests execute (the architecture work's first map).

    python3 tools/coverage.py run [--sessions-only|--lockstep-only]   instrumented runs -> build/coverage/
    python3 tools/coverage.py report [--record]                       the counts; --record: status/coverage.json

`run` builds the port with -finstrument-functions (PORT_INSTRUMENT=1 tools/undefined.py), runs
every lockstep scenario status/lockstep.json records and every UI session with $PORT_COVERAGE
set -- src/host/w16_instrument.c appends each function's address the first time it is entered
-- keeps that program as build/coverage/colonize.exe (its symbols name the addresses), and
rebuilds the ordinary one.

`report` compares the functions entered with every function the game's code defines: the text
symbols of the objects built from src/game, src/platform, src/gfx and src/asm (src/host and
src/gen are the port's own). A function counts once, by its symbol. Each is placed in the
original program's code segment by its file's `candidate for SSSS:OOOO` line, which is about
where the original source's modules fall. build/coverage/unreached.txt lists the functions no
run entered, by segment.
"""
import collections
import glob
import json
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "build", "coverage")
OBJ = os.path.join(ROOT, "build", "obj-instrument")
DIRS = ("game", "platform", "gfx", "asm")
NM = "i686-w64-mingw32-nm"
CXXFILT = ["i686-w64-mingw32-c++filt", "-_"]


def wine_path(p):
    return "Z:" + os.path.abspath(p).replace("/", "\\")


def sh(cmd, env=None, quiet=True):
    r = subprocess.run(cmd, env=env, capture_output=quiet, text=True)
    return r


def run(argv):
    if not os.environ.get("COL_GAME_DIR"):
        sys.exit("set $COL_GAME_DIR")
    shutil.rmtree(OUT, ignore_errors=True)
    os.makedirs(OUT)
    py = sys.executable
    r = sh([py, os.path.join(ROOT, "tools", "undefined.py")], env=dict(os.environ, PORT_INSTRUMENT="1"))
    if "links" not in r.stdout:
        sys.exit("the instrumented port does not link:\n" + r.stdout[-1500:] + r.stderr[-1500:])
    shutil.copyfile(os.path.join(ROOT, "build", "colonize.exe"), os.path.join(OUT, "colonize.exe"))
    try:
        if "--sessions-only" not in argv:
            led = json.load(open(os.path.join(ROOT, "status", "lockstep.json")))["scenarios"]
            for name, v in sorted(led.items()):
                save = "AUTO01.SAV" if name == "AUTO01" else os.path.join(ROOT, "build", "lockstep", "scenarios",
                                                                          name + ".SAV")
                env = dict(os.environ, LOCKSTEP_NO_BUILD="1",
                           PORT_COVERAGE=wine_path(os.path.join(OUT, "lockstep-%s.txt" % name)))
                r = sh([py, os.path.join(ROOT, "tests", "lockstep.py"), "native", save, "--turns", str(v["turns"])],
                       env=env)
                last = (r.stdout.strip().splitlines() or ["?"])[-1]
                print("lockstep %-8s %s" % (name, last))
        if "--lockstep-only" not in argv:
            env = dict(os.environ, PORT_COVERAGE=wine_path(os.path.join(OUT, "sessions.txt")))
            r = subprocess.run([py, os.path.join(ROOT, "tests", "sessions.py")], env=env, capture_output=True,
                               text=True)
            for l in r.stdout.strip().splitlines():
                print("session  " + l)
    finally:
        # the ordinary program back in build/colonize.exe
        r = sh([py, os.path.join(ROOT, "tools", "undefined.py")])
        print("rebuilt the ordinary port: %s" % ("ok" if "links" in r.stdout else "FAILED"))
    report([])


def segment_of(src):
    head = open(src, errors="replace").read(3000)
    m = re.search(r"candidate for ([0-9a-fA-F]{4}):[0-9a-fA-F]{4}", head)
    if not m:
        return "?"
    return "seg%d" % ((int(m.group(1), 16) - 0x1000) // 8 + 1)


def universe():
    """{symbol: (dir, source, segment)} for every function the game's objects define"""
    out = {}
    for d in DIRS:
        for o in sorted(glob.glob(os.path.join(OBJ, d, "*.o"))):
            src = os.path.join(ROOT, "src", d, os.path.basename(o)[:-2] + ".c")
            if not os.path.exists(src):            # an object whose source was since removed
                continue
            seg = segment_of(src)
            for l in sh([NM, "--defined-only", o]).stdout.splitlines():
                p = l.split()
                if len(p) == 3 and p[1] == "T":
                    out.setdefault(p[2], (d, os.path.relpath(src, ROOT), seg))
    return out


def report(argv):
    exe = os.path.join(OUT, "colonize.exe")
    if not os.path.exists(exe):
        sys.exit("no build/coverage/colonize.exe: run `tools/coverage.py run` first")
    addr = {}
    for l in sh([NM, exe]).stdout.splitlines():
        p = l.split()
        if len(p) == 3 and p[1] in "tT":
            addr[int(p[0], 16)] = p[2]
    runs = {}
    for f in sorted(glob.glob(os.path.join(OUT, "*.txt"))):
        if os.path.basename(f) == "unreached.txt":
            continue
        names = set()
        for l in open(f):
            l = l.strip()
            if l and int(l, 16) in addr:
                names.add(addr[int(l, 16)])
        runs[os.path.basename(f)[:-4]] = names
    all_fns = universe()
    hit = set().union(*runs.values()) & set(all_fns) if runs else set()
    lock = set().union(*[v for k, v in runs.items() if k.startswith("lockstep")]) & set(all_fns)
    sess = runs.get("sessions", set()) & set(all_fns)

    def tally(key):
        t = collections.defaultdict(lambda: [0, 0])
        for s, info in all_fns.items():
            k = key(info)
            t[k][0] += 1
            t[k][1] += s in hit
        return t

    by_dir, by_seg = tally(lambda i: i[0]), tally(lambda i: i[2])
    print("functions the game's code defines: %d; entered by a test run: %d (%.0f%%)" % (
        len(all_fns), len(hit), 100.0 * len(hit) / max(1, len(all_fns))))
    print("  by the lockstep (%d scenarios): %d; by the UI sessions: %d; by the sessions only: %d" % (
        len([k for k in runs if k.startswith("lockstep")]), len(lock), len(sess), len(sess - lock)))
    print("by source folder:")
    for k, (n, c) in sorted(by_dir.items()):
        print("  src/%-9s %5d of %5d  %3.0f%%" % (k, c, n, 100.0 * c / n))
    print("by the original program's code segment:")
    for k, (n, c) in sorted(by_seg.items(), key=lambda kv: (kv[0] == "?", int(kv[0][3:]) if kv[0][3:].isdigit() else 0)):
        print("  %-6s %5d of %5d  %3.0f%%" % (k, c, n, 100.0 * c / n))
    # the unreached, by segment, demangled
    miss = sorted((info[2], s) for s, info in all_fns.items() if s not in hit)
    dem = sh(CXXFILT + [s for _, s in miss]).stdout.splitlines() if miss else []
    with open(os.path.join(OUT, "unreached.txt"), "w") as f:
        seg = None
        for (sg, s), d in zip(miss, dem):
            if sg != seg:
                seg = sg
                f.write("\n## %s\n" % sg)
            f.write("%s  (%s)\n" % (d, all_fns[s][1]))
    print("unreached functions, by segment: %s" % os.path.relpath(os.path.join(OUT, "unreached.txt"), ROOT))
    if "--record" in argv:
        doc = {"functions": len(all_fns), "entered": len(hit), "lockstep": len(lock), "sessions": len(sess),
               "runs": sorted(runs), "by_dir": {k: {"functions": n, "entered": c} for k, (n, c) in by_dir.items()},
               "by_segment": {k: {"functions": n, "entered": c} for k, (n, c) in by_seg.items()}}
        with open(os.path.join(ROOT, "status", "coverage.json"), "w") as f:
            json.dump(doc, f, indent=1, sort_keys=True)
            f.write("\n")
        print("recorded status/coverage.json")


def main(argv):
    if not argv or argv[0] not in ("run", "report"):
        sys.exit(__doc__)
    (run if argv[0] == "run" else report)(argv[1:])


if __name__ == "__main__":
    main(sys.argv[1:])
