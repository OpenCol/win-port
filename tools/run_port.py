#!/usr/bin/env python3
"""Run the port under Wine and say how it ended (roadmap W4).

    python3 tools/run_port.py [--seconds N] [--trace]

Runs build/colonize.exe (tools/undefined.py links it) with the game directory
($COL_GAME_DIR, the patched install, hash-checked) as its working directory, so it finds
its data files where the original does. Nothing is written into the game directory:
the run's log is build/run/port.log ($PORT_LOG), where a backlog trap writes the Win16
function the game reached that the platform layer does not provide yet, and the
platform layer writes its trace (--trace sets $PORT_TRACE).

Exit status: 0 the game exited by itself, 3 it reached a trap (the log names it),
124 it was still running after --seconds (then stopped), other: it crashed.
"""
import os
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
    game = colwinre.game_dir()
    os.makedirs(RUN, exist_ok=True)
    log = os.path.join(RUN, "port.log")
    if os.path.exists(log):
        os.remove(log)
    env = dict(os.environ, WINEDEBUG=os.environ.get("WINEDEBUG", "-all"), PORT_LOG=log)
    if "--trace" in argv:
        env["PORT_TRACE"] = "1"
    t0 = time.time()
    p = subprocess.Popen([wine(), EXE], cwd=game, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    try:
        out, _ = p.communicate(timeout=secs)
        code = p.returncode
    except subprocess.TimeoutExpired:
        p.kill()
        out, _ = p.communicate()
        subprocess.run(["wineserver", "-k"], capture_output=True)
        code = 124
    text = open(log).read() if os.path.exists(log) else ""
    how = {0: "exited", 3: "reached a trap", 124: "still running after %ds (stopped)" % secs}.get(code, "crashed (%d)" % code)
    print("colonize.exe %s in %.1f s" % (how, time.time() - t0))
    tail = [l for l in text.splitlines() if l.startswith("unported")]
    if tail:
        print("  " + tail[-1])
    if "-v" in argv:
        print(text[-4000:])
        print(out.decode(errors="replace")[-2000:])
    return code


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
