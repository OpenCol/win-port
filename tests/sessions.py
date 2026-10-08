#!/usr/bin/env python3
"""Scripted UI sessions on the port (roadmap W7): every screen the game has, opened the way a
player opens it, from a saved game, and looked at.

    python3 tests/sessions.py [NAME ...]           run the sessions (all, or those named)
    python3 tests/sessions.py --record             also write status/sessions.json
    python3 tests/sessions.py --saves [--record]   saves both ways: the port's save (the `save` session)
                                                   loaded by the original and played on in lockstep
                                                   (tests/lockstep.py, 5 turns); the original's saves
                                                   loaded by the port are every lockstep scenario's
                                                   (AUTO01.SAV) -> status/saves.json

A session is the lockstep's script (src/host/w16_lockstep.c) with the player left human
(`noautoplay`): the game loads SAVE through its own menu, then each step runs once the game sits
idle -- a menu command (WM_COMMAND, the main window's own: MainWin_MainMenuProc), a key, a
click, a picture of the screen as Wine shows it (`grab`, a .bmp in build/sessions/NAME/, with
NAME.txt listing the windows), a wait -- and `exit` ends it. `new_game` starts from the main
menu instead of a save: America, the first leader, the first nation, the King's audience --
with sound on (`sound`): the first WAV plays there, and a Wine that cannot play one stops.
A session passes when it reaches `exit`: no crash, no Win16 function the platform layer lacks
(an `unported` trap), no stall (still running at --seconds). The snapshots are for a person to
look at, and for the comparison with the original (to come: the oracle runs the same scripts).
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

OUT = os.path.join(ROOT, "build", "sessions")
RETURN, ESCAPE, DOWN = 0x0D, 0x1B, 0x28
LOAD = [("key", DOWN), ("key", DOWN), ("key", DOWN), ("key", RETURN), ("wait", 4), ("grab", "loaded")]
OK = (112, 123)                          # the Ok of the difficulty and nation screens, in their window


def show(cmd, name, close=ESCAPE):
    """a menu command's screen: opened, looked at, closed"""
    return [("cmd", cmd), ("wait", 3), ("grab", name), ("key", close), ("wait", 2)]


SESSIONS = {
    # the map and its views
    "map": [("cmd", 306), ("wait", 2), ("grab", "zoom_in"), ("cmd", 308), ("wait", 2), ("grab", "zoom_out"),
            ("cmd", 307), ("wait", 2), ("cmd", 310), ("wait", 2), ("grab", "reveal"), ("cmd", 311), ("wait", 2),
            ("cmd", 312), ("wait", 2), ("grab", "default_desktop")],
    "europe": show(302, "europe"),
    "reports": sum((show(c, "report_%d" % c, RETURN) for c in range(501, 510)), []),
    "colonopedia": sum((show(c, "pedia_%d" % c) for c in range(700, 706)), []) + show(500, "pedia_terrain"),
    "about": show(706, "about", RETURN),
    "options": show(100, "game_options", RETURN) + show(101, "colony_options", RETURN) + show(55, "control_panel"),
    "find_colony": show(303, "find_city"),
    "save": [("saveas", "SESSION.SAV"), ("cmd", 104), ("wait", 4), ("grab", "saved")],
    "new_game": [("wait", 3), ("grab", "main_menu"), ("key", DOWN), ("key", RETURN), ("wait", 4), ("grab", "difficulty"),
                 ("click", OK), ("wait", 4), ("grab", "nation"), ("click", OK), ("wait", 4), ("grab", "name")]
                + sum(([("key", RETURN), ("wait", 6), ("grab", "start_%d" % i)] for i in range(1, 6)), [])
                + [("wait", 10), ("grab", "map")],
}
FROM_MENU = {"new_game"}


def run(name, steps, save="AUTO01.SAV", seconds=180):
    work = os.path.join(OUT, name)
    if os.path.isdir(work):
        shutil.rmtree(work)
    os.makedirs(work)
    path = save if os.path.isabs(save) else os.path.join(colwinre.game_dir(), save)
    wine = lambda p: "Z:" + os.path.abspath(p).replace("/", "\\")
    lines = ["save " + wine(path), "noautoplay"] + (["sound"] if name in FROM_MENU else [])
    for kind, v in ([] if name in FROM_MENU else LOAD) + steps + [("exit", "")]:
        if kind == "saveas":
            lines.append("saveas " + wine(os.path.join(work, v)))
        elif kind == "key":
            lines.append("key %#x" % v)
        elif kind == "click":
            lines.append("click %d %d" % v)
        else:
            lines.append("%s %s" % (kind, v))
    open(os.path.join(work, "lockstep.txt"), "w").write("\n".join(lines) + "\n")
    t0 = time.time()
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "run_port.py"), "--seconds", str(seconds),
                        "--snapshot"], env=dict(os.environ, PORT_LOCKSTEP=wine(work), PORT_RUN_DIR=os.path.join(work, "run")),
                       capture_output=True, text=True)
    log = open(os.path.join(work, "native.log")).read() if os.path.exists(os.path.join(work, "native.log")) else ""
    plog = os.path.join(work, "run", "port.log")
    port_log = open(plog, errors="replace").read() if os.path.exists(plog) else ""
    snaps = sorted(f[:-4] for f in os.listdir(work) if f.endswith(".bmp"))
    if "session done" in log:
        verdict = "pass"
    elif "unported" in port_log:
        verdict = "unported: " + next(l for l in port_log.splitlines() if "unported" in l)[:120]
    elif "crash" in port_log or r.returncode not in (0, 124):
        verdict = "crash (exit %d): %s" % (r.returncode, (next((l for l in port_log.splitlines() if "crash" in l or "fault" in l), "")[:120]))
    else:
        done = sum(log.count("\n%s " % k) for k in ("cmd", "key", "snap", "grab", "click"))
        verdict = "stalled after %d steps" % done
    return {"verdict": verdict, "snapshots": snaps, "seconds": round(time.time() - t0)}


def saves(record):
    r = run("save", SESSIONS["save"])
    src = os.path.join(OUT, "save", "SESSION.SAV")
    if r["verdict"] != "pass" or not os.path.exists(src):
        sys.exit("the save session did not save: %s" % r["verdict"])
    dst = os.path.join(ROOT, "build", "lockstep", "scenarios", "PORTSAVE.SAV")
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    shutil.copyfile(src, dst)
    p = subprocess.run([sys.executable, os.path.join(ROOT, "tests", "lockstep.py"), "run", dst, "--turns", "5", "--record"],
                       capture_output=True, text=True)
    print(p.stdout.strip().splitlines()[-1] if p.stdout.strip() else p.stderr[-500:])
    led = json.load(open(os.path.join(ROOT, "status", "lockstep.json")))["scenarios"]
    doc = {"port_to_original": led.get("PORTSAVE"), "original_to_port": led.get("AUTO01")}
    ok = all(v and v["in_lockstep"] == v["checkpoints"] and v["checkpoints"] for v in doc.values())
    doc["ok"] = ok
    if record:
        with open(os.path.join(ROOT, "status", "saves.json"), "w") as f:
            json.dump(doc, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if ok else 1


def main(argv):
    if "--saves" in argv:
        return saves("--record" in argv)
    names = [a for a in argv if not a.startswith("--")] or list(SESSIONS)
    res = {}
    for n in names:
        res[n] = run(n, SESSIONS[n])
        print("%-12s %-8s %3ds  %d snapshots" % (n, res[n]["verdict"][:60], res[n]["seconds"], len(res[n]["snapshots"])))
    if "save" in res and res["save"]["verdict"] == "pass":
        ok = os.path.exists(os.path.join(OUT, "save", "SESSION.SAV"))
        print("save: SESSION.SAV %s" % ("written" if ok else "MISSING"))
    if "--record" in argv:
        with open(os.path.join(ROOT, "status", "sessions.json"), "w") as f:
            json.dump({n: {"verdict": r["verdict"], "snapshots": len(r["snapshots"])} for n, r in res.items()}, f,
                      indent=1, sort_keys=True)
            f.write("\n")
    return 0 if all(r["verdict"] == "pass" for r in res.values()) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
