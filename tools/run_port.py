#!/usr/bin/env python3
"""Run the port under Wine and say how it ended (roadmap W4).

    python3 tools/run_port.py [--seconds N] [--trace] [--desktop WxH]

Runs build/colonize.exe (tools/undefined.py links it) in a fresh copy of the game directory
($COL_GAME_DIR, the patched install, hash-checked; copied to build/run/game every run), so it
finds its data files where the original does and whatever it writes -- the game autosaves
(AUTO01.SAV, auto02.sav) -- lands in the copy. Nothing is written into the game directory:
the run's log is build/run/port.log ($PORT_LOG), where a backlog trap writes the Win16
function the game reached that the platform layer does not provide yet, and the
platform layer writes its trace (--trace sets $PORT_TRACE).

Exit status: 0 the game exited by itself, 3 it reached a trap (the log names it),
124 it was still running after --seconds (then stopped), other: it crashed.
"""
import os
import re
import shutil
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import colwinre               # noqa: E402

EXE = os.path.join(ROOT, "build", "colonize.exe")
RUN = os.path.join(ROOT, "build", "run")


def wine():
    for w in ("wine", "wine64"):
        if shutil.which(w):
            return shutil.which(w)
    sys.exit("no wine on PATH")


def main(argv):
    secs = int(argv[argv.index("--seconds") + 1]) if "--seconds" in argv else 20
    os.makedirs(RUN, exist_ok=True)
    game = os.path.join(RUN, "game")
    if os.path.isdir(game):
        shutil.rmtree(game)
    shutil.copytree(colwinre.game_dir(), game)
    log = os.path.join(RUN, "port.log")
    if os.path.exists(log):
        os.remove(log)
    env = dict(os.environ, WINEDEBUG=os.environ.get("WINEDEBUG", "-all,err+seh"), PORT_LOG=log)
    if "--trace" in argv:
        env["PORT_TRACE"] = "1"
    if "--snapshot" in argv:
        # Wine sees the host filesystem as Z:
        for old in os.listdir(RUN):
            if old.startswith("frame") and old.endswith(".bmp"):
                os.remove(os.path.join(RUN, old))
        env["PORT_SNAPSHOT"] = "Z:" + RUN.replace("/", "\\")
    t0 = time.time()
    # a Wine virtual desktop: windows go where the program puts them, as on Windows 3.1, with no
    # host window manager moving them (the macOS menu bar would push one created at y=0 down)
    size = argv[argv.index("--desktop") + 1] if "--desktop" in argv else "1024x768"
    cmd = [wine(), "explorer", "/desktop=colonization," + size, EXE] if "--no-desktop" not in argv else [wine(), EXE]
    p = subprocess.Popen(cmd, cwd=game, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    try:
        out, _ = p.communicate(timeout=secs)
        code = p.returncode
    except subprocess.TimeoutExpired:
        # the whole Wine session: in a virtual desktop the game is not p's own process, and
        # holds p's output open
        subprocess.run(["wineserver", "-k"], capture_output=True)
        p.kill()
        try:
            out, _ = p.communicate(timeout=20)
        except subprocess.TimeoutExpired:
            out = b""
        code = 124
    text = open(log).read() if os.path.exists(log) else ""
    how = {0: "exited", 3: "reached a trap", 124: "still running after %ds (stopped)" % secs}.get(code, "crashed (%d)" % code)
    print("colonize.exe %s in %.1f s" % (how, time.time() - t0))
    tail = [l for l in text.splitlines() if l.startswith("unported")]
    if tail:
        print("  " + tail[-1])
    out_s = out.decode(errors="replace")
    # a crash: Wine names the faulting address; the build has debug info and a fixed base
    m = re.search(r"Unhandled (\w[\w ]*?) (?:on (\w+) access to ([0-9a-fA-F]+) )?at address ([0-9a-fA-F]+)", out_s)
    if m:
        where = subprocess.run(["i686-w64-mingw32-addr2line", "-f", "-C", "-e", EXE, m.group(4)],
                               capture_output=True, text=True).stdout.split()
        print("  %s%s at %s: %s" % (m.group(1), " (%s %s)" % (m.group(2), m.group(3)) if m.group(2) else "",
                                   m.group(4), " ".join(where)))
    for label, pat in (("crash", r"^crash (code|at|address) ([0-9a-f]+)"),):
        got = dict(re.findall(pat, text, re.M))
        if got:
            where = subprocess.run(["i686-w64-mingw32-addr2line", "-f", "-C", "-s", "-e", EXE, got.get("at", "0")],
                                   capture_output=True, text=True).stdout.split()
            print("  fault %s at %s %s%s" % (got.get("code"), got.get("at"), " ".join(where),
                                            " reading/writing %s" % got["address"] if "address" in got else ""))
    entered = re.findall(r"^entered ([0-9a-f]+)", text, re.M)
    if entered:
        r = subprocess.run(["i686-w64-mingw32-addr2line", "-f", "-C", "-s", "-e", EXE] + entered,
                           capture_output=True, text=True).stdout.splitlines()
        print("  last game functions entered, newest first:")
        for i in range(0, min(len(r) - 1, 40), 2):
            print("    %-36s %s" % (r[i], r[i + 1]))
    frames = re.findall(r"^frame (?:0x)?([0-9a-fA-F]+)", text, re.M)
    if frames:
        r = subprocess.run(["i686-w64-mingw32-addr2line", "-f", "-C", "-s", "-e", EXE] + frames,
                           capture_output=True, text=True).stdout.splitlines()
        print("  called from:")
        for i in range(0, len(r) - 1, 2):
            print("    %-36s %s" % (r[i], r[i + 1]))
    calls = [l for l in text.splitlines() if not l.startswith(("unported", "crash", "frame", "entered"))]
    if calls and ("--trace" in argv):
        print("  last platform calls:")
        for l in calls[-8:]:
            print("    " + l)
    if "-v" in argv:
        print(text[-4000:])
        print(out.decode(errors="replace")[-2000:])
    return code


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
