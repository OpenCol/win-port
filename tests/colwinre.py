"""col-win-re's emulator tools, at the commit in import/ORACLE, for the W3-W6 tests.

The tools have their own pin, apart from the corpus's (import/SOURCE): the emulator is a test
instrument, and a fix to it (col-win-re 0ad06e0da: MoveWindow's messages, window frames, the
border metric) must not wait for the corpus's three-way merge. Without import/ORACLE, SOURCE.

The oracle and the state tests use col-win-re's software Win16 machine
(tools/re_agent/win16emu.py, winsim.py, probe.py, state.py) as a library. Like the import,
they never read col-win-re's working tree: the pinned commit is unpacked with `git archive`
into build/colwinre/<commit>/ once, and the game directory is linked into it as `game/`.

The game files come from $COL_GAME_DIR and are never committed. Only the patched 1995
build is accepted: COLONIZE.EXE must have the SHA-256 below.

The emulator needs unicorn: build/oracle-venv is made on first use (pip install unicorn).
"""
import hashlib
import os
import subprocess
import sys
import tarfile
import io

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PATCHED_EXE = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
UNICORN = "unicorn==2.1.4"
# what the tools read from col-win-re itself (no game bytes: those come from $COL_GAME_DIR)
# Without the ordinal tables the machine has no handler for KERNEL.ORD_18 and crawls, one
# instruction per run: every file the emulator tools read is here.
PATHS = ["tools", "analysis/model.json", "analysis/ne", "analysis/save-layout.json",
         "analysis/win16-ordinals.json", "analysis/colonize-imports-resolved.json",
         "analysis/ghidra/functions.json", "analysis/dyn-trace.json", "analysis/re-wmcommand.json",
         "analysis/re_agent/runtime"]


def col_win_re():
    for p in (os.environ.get("COL_WIN_RE"), os.path.join(ROOT, "..", "..", "dderevjanik", "col-win-re")):
        if p and os.path.isdir(os.path.join(p, ".git")):
            return os.path.abspath(p)
    sys.exit("col-win-re not found: set $COL_WIN_RE")


def game_dir():
    d = os.environ.get("COL_GAME_DIR")
    if not d:
        sys.exit("set $COL_GAME_DIR to the patched Colonization install (COLONIZE.EXE, COLDATA*.DLL, ...)")
    exe = os.path.join(d, "COLONIZE.EXE")
    if not os.path.exists(exe):
        sys.exit("$COL_GAME_DIR has no COLONIZE.EXE")
    h = hashlib.sha256(open(exe, "rb").read()).hexdigest()
    if h != PATCHED_EXE:
        sys.exit("$COL_GAME_DIR/COLONIZE.EXE is not the patched 1995 build (sha256 %s)" % h)
    return os.path.abspath(d)


def tree():
    """The unpacked col-win-re tools at SOURCE, with game/ linked to $COL_GAME_DIR."""
    pin = os.path.join(ROOT, "import", "ORACLE")
    source = open(pin if os.path.exists(pin) else os.path.join(ROOT, "import", "SOURCE")).read().strip()
    out = os.path.join(ROOT, "build", "colwinre", source[:12])
    if not os.path.isdir(os.path.join(out, "tools")):
        os.makedirs(out, exist_ok=True)
        data = subprocess.run(["git", "-C", col_win_re(), "archive", source, "--"] + PATHS,
                              capture_output=True, check=True).stdout
        with tarfile.open(fileobj=io.BytesIO(data)) as t:
            t.extractall(out)
    link = os.path.join(out, "game")
    want = game_dir()
    if os.path.islink(link) and os.readlink(link) != want:
        os.remove(link)
    if not os.path.exists(link):
        os.symlink(want, link)
    return out


def python():
    """A Python with unicorn, for running the emulator."""
    venv = os.path.join(ROOT, "build", "oracle-venv")
    py = os.path.join(venv, "bin", "python")
    if not os.path.exists(py):
        subprocess.run([sys.executable, "-m", "venv", venv], check=True)
        subprocess.run([os.path.join(venv, "bin", "pip"), "install", "-q", UNICORN], check=True)
    return py


def use():
    """Put the col-win-re tools on sys.path (in a process that already has unicorn)."""
    t = tree()
    for p in (os.path.join(t, "tools", "re_agent"), os.path.join(t, "tools")):
        if p not in sys.path:
            sys.path.insert(0, p)
    return t


# Wine: the runtime col-win-re set up (tools/setup-runtime.sh: Gcenx wine-devel 11.18), not the
# first on PATH. Apple's GPTK Wine 7.7 (Homebrew's wine64) aborts the whole process in
# alloc_pages_vprot (virtual.c) the first time a sound plays -- sndPlaySound, sync or async; a
# new game's first is meetking.wav, after the King's audience. $PORT_WINE overrides.
WINE11 = os.path.join("analysis", "dynamic", "runtime", "wine-11.18", "Wine Devel.app", "Contents", "Resources",
                      "wine", "bin", "wine")


# $PORT_TARGET: which build the tests run -- win32 (build/colonize.exe, MinGW, under Wine on this
# machine), x64 (build/x64/colonize.exe, 64-bit Windows, W9, the same Wine: macOS's 64-bit
# target), linux64 (the same program under the Linux machine's 64-bit Wine: Linux x86_64) or linux (build/linux/colonize.exe.so, Winelib, roadmap W8: under Wine in the
# OrbStack Linux machine, tools/linux/wine). Each target has its own run directories and
# status records (suffix "-linux").
def target():
    t = os.environ.get("PORT_TARGET", "win32")
    if t not in ("win32", "linux", "x64", "linux64"):
        sys.exit("PORT_TARGET: win32, linux, x64 or linux64")
    return t


def suffix():
    return "" if target() == "win32" else "-" + target()


def port_exe():
    if os.environ.get("PORT_EXE"):
        return os.path.abspath(os.environ["PORT_EXE"])
    if target() == "linux":
        return os.path.join(ROOT, "build", "linux", "colonize.exe.so")
    if target() in ("x64", "linux64"):
        return os.path.join(ROOT, "build", "x64", "colonize.exe")
    return os.path.join(ROOT, "build", "colonize.exe")


def build_command():
    """the command that builds the target's program (tools/undefined.py; for linux, in the machine)"""
    if target() == "linux":
        return ["orb", "-m", os.environ.get("PORT_LINUX_MACHINE", "colw8"), "-w", ROOT, "env", "PORT_TOOLCHAIN=winegcc",
                "python3", os.path.join(ROOT, "tools", "undefined.py")]
    if target() in ("x64", "linux64"):
        return ["env", "PORT_TOOLCHAIN=mingw-x64", sys.executable, os.path.join(ROOT, "tools", "undefined.py")]
    return [sys.executable, os.path.join(ROOT, "tools", "undefined.py")]


def wine():
    import shutil
    if os.environ.get("PORT_WINE"):
        return os.environ["PORT_WINE"]
    if target() in ("linux", "linux64"):
        return os.path.join(ROOT, "tools", "linux", "wine")
    for p in (os.environ.get("COL_WIN_RE"), os.path.join(ROOT, "..", "..", "dderevjanik", "col-win-re")):
        if p and os.path.exists(os.path.join(p, WINE11)):
            return os.path.abspath(os.path.join(p, WINE11))
    for w in ("wine", "wine64"):
        if shutil.which(w):
            sys.stderr.write("warning: Wine 11 not found (col-win-re tools/setup-runtime.sh); using %s, which may "
                             "abort when a sound plays\n" % shutil.which(w))
            return shutil.which(w)
    sys.exit("no Wine: run col-win-re's tools/setup-runtime.sh, or set $PORT_WINE")


def wine_env(env=None):
    """the environment a Wine run gets: the port's own prefix (build/wineprefix), not the
    user's ~/.wine nor col-win-re's, unless $WINEPREFIX names one"""
    env = dict(os.environ if env is None else env)
    env.setdefault("WINEPREFIX", os.path.join(ROOT, "build", "wineprefix"))
    return env


def wineserver():
    w = wine()
    s = os.path.join(os.path.dirname(w), "wineserver")
    return s if os.path.exists(s) else "wineserver"
