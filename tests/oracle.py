#!/usr/bin/env python3
"""The oracle: the shipped COLONIZE.EXE playing itself, recorded per turn step (roadmap W3).

    python3 tests/oracle.py record NAME [--turns N] [--seed S]   -> build/oracle/NAME.json (+ .raw.gz)
    python3 tests/oracle.py compare A B                           two recordings, offline
    python3 tests/oracle.py --selftest [--turns N] [--record]     the gate: two runs, identical

It runs col-win-re's software Win16 machine (win16emu + winsim) through e2e_turns.py,
the recorder col-win-re wrote and has not committed yet: win-port carries a copy
(tests/oracle/e2e_turns.py) and runs it from the col-win-re tree unpacked at
import/SOURCE (tests/colwinre.py), with the game from $COL_GAME_DIR.

The game plays itself: the new-game dialogs are answered at fixed emulator ticks, every
nation is set to the computer and g_autoplay on, and at each turn step (natives, the four
viceroys, the end of turn) the recorder takes the 57 save_game blocks, the C runtime's
rand state, the Win16 API calls since the last step and the screen. The machine has no
wall clock and no host randomness, so a run is a function of its inputs; --selftest
checks that, by recording twice and comparing.

Recordings stay out of git (build/oracle/); their hashes go in tests/golden/MANIFEST
once the golden corpus is recorded.
"""
import json
import os
import shutil
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import colwinre               # noqa: E402

OUT = os.path.join(ROOT, "build", "oracle")


def recorder():
    t = colwinre.tree()
    dst = os.path.join(t, "tools", "e2e_turns.py")
    shutil.copyfile(os.path.join(ROOT, "tests", "oracle", "e2e_turns.py"), dst)
    return t, dst


def record(name, turns, seed=None):
    t, rec = recorder()
    os.makedirs(OUT, exist_ok=True)
    out, raw = os.path.join(OUT, name + ".json"), os.path.join(OUT, name + ".raw.gz")
    cmd = [colwinre.python(), "-u", rec, "record", "--exe", os.path.join(t, "game", "COLONIZE.EXE"),
           "--turns", str(turns), "--out", out, "--raw", raw]
    if seed is not None:
        cmd += ["--seed", str(seed)]
    t0 = time.time()
    log = os.path.join(OUT, name + ".log")       # progress, as it happens: a stall shows here
    with open(log, "w") as f:
        code = subprocess.run(cmd, cwd=t, stdout=f, stderr=subprocess.STDOUT).returncode
    if code:
        sys.exit("recording %s failed (%d):\n%s" % (name, code, open(log).read()[-3000:]))
    return out, raw, time.time() - t0


def compare(a, b, raws=None):
    t, rec = recorder()
    cmd = [colwinre.python(), rec, "compare", a, b] + (["--raw"] + list(raws) if raws else [])
    r = subprocess.run(cmd, cwd=t, capture_output=True, text=True)
    return r.returncode, r.stdout + r.stderr


def checkpoints(path):
    d = json.load(open(path))
    for k in ("checkpoints", "steps", "records"):
        if isinstance(d.get(k), list):
            return len(d[k])
    return 0


def main(argv):
    def opt(n, default=None):
        return argv[argv.index(n) + 1] if n in argv else default
    turns = int(opt("--turns", "1"))
    if argv[:1] == ["record"]:
        out, _raw, secs = record(argv[1], turns, opt("--seed"))
        print("%s: %d checkpoints in %.0f s" % (os.path.relpath(out, ROOT), checkpoints(out), secs))
        return 0
    if argv[:1] == ["compare"]:
        code, text = compare(argv[1], argv[2])
        print(text)
        return code
    if "--selftest" in argv:
        a, ra, sa = record("selftest-a", turns)
        b, rb, sb = record("selftest-b", turns)
        code, text = compare(a, b, (ra, rb))
        n = checkpoints(a)
        ok = code == 0 and n > 0
        print(text.strip()[-2000:])
        print("selftest: %s -- %d turns, %d checkpoints, %.0f s + %.0f s" % (
            "identical" if ok else "DIFFERENT", turns, n, sa, sb))
        if "--record" in argv:
            with open(os.path.join(ROOT, "status", "oracle.json"), "w") as f:
                json.dump({"ok": ok, "turns": turns, "checkpoints": n, "seconds": round(sa + sb)},
                          f, indent=1, sort_keys=True)
                f.write("\n")
        return 0 if ok else 1
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
