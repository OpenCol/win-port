#!/usr/bin/env python3
"""One command that says whether the port still behaves as it did: the refactoring's gate.

    python3 tools/gate.py            quick (a few minutes): after every step
    python3 tools/gate.py --full     every lockstep scenario and every UI session: after a group of steps

Both build the port (Windows x86, tools/undefined.py), then run, stopping at nothing -- every
check's line is printed, and the last line is `GATE: ok` or `GATE: FAILED (names)`; a script
should look for `GATE: ok` (a pipe through tail hides the exit code):

  consistency  import/import.py --check (each imported file is col-win-re's, or marked PORT:),
               tools/gen_headers.py --check, tools/gen_exe_data.py --check, tools/status.py --check
  lockstep     the port against the original game, checkpoint by checkpoint (tests/lockstep.py):
               quick AUTO01 and NEW001, --full every scenario status/lockstep.json records
  sessions     the UI sessions, each screen against its baseline pixel for pixel (tests/sessions.py):
               quick map, colony and about, --full all of them

A failing run can be slow: a port that no longer reaches the original's checkpoints plays on until
the lockstep's time limit (a deliberate break of colony_has_building took 32 minutes, against
about 2 for a passing quick run). import/import.py --check rewrites status/import.json, so after a
failure that touched an imported file, the next passing run puts it back.

Not here: tools/gen_globals.py --check, which fails on a stale reading of its own (it would define
eight Sprite globals src/game/create_game_globals.c already defines); the per-function
differential (tests/diff.py, W3); the other targets (x64, Linux).
"""
import json
import os
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PY = sys.executable
QUICK_LOCKSTEP = ("AUTO01", "NEW001")
QUICK_SESSIONS = ("map", "colony", "about")


def step(name, cmd, ok, env=None, timeout=3600):
    """run cmd; ok(stdout) says whether it passed; one line either way"""
    t0 = time.time()
    try:
        r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, env=env, timeout=timeout)
        out = r.stdout + r.stderr
        good = r.returncode == 0 and ok(r.stdout)
    except subprocess.TimeoutExpired:
        out, good = "timed out after %ds" % timeout, False
    last = [l for l in out.strip().splitlines() if l.strip()][-1:] or [""]
    print("%-4s %-34s %4ds  %s" % ("ok" if good else "FAIL", name, time.time() - t0, last[0][:110]), flush=True)
    if not good and out.strip():
        for l in out.strip().splitlines()[-8:]:
            print("       | " + l[:150])
    return good


def scenario_save(name):
    return "AUTO01.SAV" if name == "AUTO01" else os.path.join(ROOT, "build", "lockstep", "scenarios", name + ".SAV")


def main(argv):
    full = "--full" in argv
    if not os.environ.get("COL_GAME_DIR"):
        sys.exit("set $COL_GAME_DIR (the patched game's folder)")
    if os.environ.get("PORT_TARGET", "win32") != "win32":
        sys.exit("the gate measures the Windows x86 program (unset PORT_TARGET)")
    t0 = time.time()
    failed = []

    def check(name, *a, **k):
        if not step(name, *a, **k):
            failed.append(name)

    print("gate (%s), %s" % ("full" if full else "quick", time.strftime("%H:%M:%S")), flush=True)
    check("build", [PY, "tools/undefined.py"], lambda o: "links" in o)
    if "build" in failed:
        print("GATE: FAILED (build)")
        return 1
    check("import is col-win-re's + PORT:", [PY, "import/import.py", "--check"], lambda o: "REPRODUCED" in o)
    check("include/gen current", [PY, "tools/gen_headers.py", "--check"], lambda o: "current" in o)
    check("src/gen/exe_data.c current", [PY, "tools/gen_exe_data.py", "--check"], lambda o: "current" in o)
    check("STATUS.md current", [PY, "tools/status.py", "--check"], lambda o: "current" in o)

    led = json.load(open(os.path.join(ROOT, "status", "lockstep.json")))["scenarios"]
    names = sorted(led) if full else [n for n in QUICK_LOCKSTEP if n in led]
    env = dict(os.environ, LOCKSTEP_NO_BUILD="1")    # built above: no relink per scenario
    for n in names:
        save, turns = scenario_save(n), str(led[n]["turns"])
        if not step("lockstep %s: native" % n, [PY, "tests/lockstep.py", "native", save, "--turns", turns],
                    lambda o: "checkpoints" in o, env=env):
            failed.append("lockstep " + n)
            continue
        want = "%d of %d checkpoints in lockstep" % (led[n]["checkpoints"], led[n]["checkpoints"])
        check("lockstep %s: compare" % n, [PY, "tests/lockstep.py", "compare", save], lambda o, w=want: w in o)

    sessions = [] if full else list(QUICK_SESSIONS)
    check("sessions (%s)" % ("all" if full else ", ".join(sessions)), [PY, "tests/sessions.py"] + sessions,
          lambda o: o.strip() and all(" pass " in l or l.startswith("save:") for l in o.strip().splitlines()))
    print("%s (%ds)" % ("GATE: ok" if not failed else "GATE: FAILED (%s)" % ", ".join(failed), time.time() - t0))
    return 0 if not failed else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
