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

OUT = os.path.join(ROOT, "build", "sessions" + colwinre.suffix())
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
    # a colony's screen. AUTO01's only colony is the Dutch New Amsterdam, which the game opens for the
    # English player once the map is revealed: Cheat > Reveal Map (802), Complete Map (the fifth
    # choice); then Find Colony (303) takes the name and puts the map cursor on it, and Return opens
    # it (GameBrains_KeyboardEvent, key 0xd0); its own Escape closes it
    "colony": [("cmd", 802), ("wait", 2)] + [("key", DOWN)] * 4 + [("key", RETURN), ("wait", 3), ("cmd", 303),
              ("wait", 2), ("type", "New Amsterdam"), ("key", RETURN), ("wait", 3), ("grab", "found"),
              ("key", RETURN), ("wait", 4), ("grab", "colony"), ("key", ESCAPE), ("wait", 3), ("grab", "closed")],
    # the Jukebox's song list: a Popup in g_popup_font_small (g_font12), which v0.1.1 pointed at a
    # copy of the file's bytes -- it crashed on Windows 7 (src/host/w16_globals_init.c, map_far)
    "jukebox": show(103, "by_song"),
    # every other menu command once (MainWin_MainMenuProc's ids, src/gen/exe_data.c's menus): a
    # player meets each, and a tester found the crashes one by one; Load Game is the save session's
    # dialog, Quit and Retire ask first (Escape answers no)
    "file_menu": show(102, "sound_options", RETURN) + show(106, "declare", ESCAPE) + show(107, "retire", ESCAPE)
                 + show(108, "quit", ESCAPE),
    "view_menu": sum((show(c, "view_%d" % c) for c in (300, 301, 304, 305)), []),
    # orders that end the unit's moves (402 Fortify, 404 Sentry, 413 Return to Europe, 414 No Orders)
    # are left out: with the only unit done the game plays its turns on by itself, and the session
    # never gets an idle moment for its next key
    "orders_menu": sum((show(c, "order_%d" % c) for c in (400, 405, 406, 407, 408, 409, 411, 412, 415, 416, 401)), []),
    "trade_routes": sum((show(c, "route_%d" % c) for c in (600, 601, 602)), []),
    "cheat_menu": sum((show(c, "cheat_%d" % c) for c in (801, 802, 803, 806, 807, 808, 809, 800, 805, 804, 810)), []),
    "save": [("saveas", "SESSION.SAV"), ("cmd", 104), ("wait", 4), ("grab", "saved")],
    "new_game": [("wait", 3), ("grab", "main_menu"), ("key", DOWN), ("key", RETURN), ("wait", 4), ("grab", "difficulty"),
                 ("click", OK), ("wait", 4), ("grab", "nation"), ("click", OK), ("wait", 4), ("grab", "name")]
                + sum(([("key", RETURN), ("wait", 6), ("grab", "start_%d" % i)] for i in range(1, 6)), [])
                + [("wait", 10), ("grab", "map")],
}
FROM_MENU = {"new_game"}

# sessions that start from another save than AUTO01.SAV (none yet). A save made from a
# lockstep checkpoint (tests/lockstep.py makesave) is no good for a session: written mid-turn
# with every nation the computer's, it plays its turns on by itself to 1800 even with England
# made human again and a unit left without orders (tried 2026-10-12).
SAVES = {}


def save_for(name):
    return SAVES.get(name, "AUTO01.SAV")


def run(name, steps, save="AUTO01.SAV", seconds=int(os.environ.get("PORT_SESSION_SECONDS", "180"))):
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
        elif kind == "type":
            lines.append("type " + v)
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
        res[n] = run(n, SESSIONS[n], save=save_for(n))
        print("%-12s %-8s %3ds  %d snapshots" % (n, res[n]["verdict"][:60], res[n]["seconds"], len(res[n]["snapshots"])))
    if "save" in res and res["save"]["verdict"] == "pass":
        ok = os.path.exists(os.path.join(OUT, "save", "SESSION.SAV"))
        print("save: SESSION.SAV %s" % ("written" if ok else "MISSING"))
    if "--record" in argv:
        # all sessions: the record is this run's; named ones: merged into the record, so a session
        # re-run alone (one that ran out of time on a loaded machine) replaces only its own line
        path = os.path.join(ROOT, "status", "sessions%s.json" % colwinre.suffix())
        doc = {}
        if names != list(SESSIONS) and os.path.exists(path):
            doc = json.load(open(path))
        doc.update({n: {"verdict": r["verdict"], "snapshots": len(r["snapshots"])} for n, r in res.items()})
        with open(path, "w") as f:
            json.dump(doc, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if all(r["verdict"] == "pass" for r in res.values()) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
