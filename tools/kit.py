#!/usr/bin/env python3
"""The release kit (roadmap W10): one zip per platform, the port and a launcher, no game files.

    python3 tools/kit.py [TARGET ...] [--version V] [--record]     targets: windows-x86 windows-x64 linux-i386 macos
                                                          (default: every target whose program is built)

    windows-x86   Colonization.exe (the launcher) + bin/colonize.exe       (tools/undefined.py)
    windows-x64   the same, 64-bit                                          (PORT_TOOLCHAIN=mingw-x64)
    linux-i386    colonization (the launcher script) + bin/colonize.exe.so  (PORT_TOOLCHAIN=winegcc, Winelib)
    macos         colonization + bin/colonize.exe, run under Wine           (the Windows x86 program)

The launchers (src/launcher) find the player's copy of the game, check COLONIZE.EXE's SHA-256
against the updated 1995 release, and start the port in that folder. Writes dist/*.zip.
"""
import os
import shutil
import subprocess
import sys
import time
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIST = os.path.join(ROOT, "dist")
LAUNCHER_C = os.path.join(ROOT, "src", "launcher", "launcher.c")
LAUNCHER_SH = os.path.join(ROOT, "src", "launcher", "colonization.sh")

TARGETS = {
    # target: (program, its name in bin/, launcher kind, mingw prefix for the launcher/strip)
    "windows-x86": ("build/colonize.exe", "colonize.exe", "exe", "i686-w64-mingw32"),
    "windows-x64": ("build/x64/colonize.exe", "colonize.exe", "exe", "x86_64-w64-mingw32"),
    "linux-i386": ("build/linux/colonize.exe.so", "colonize.exe.so", "sh", None),
    "macos": ("build/colonize.exe", "colonize.exe", "sh", "i686-w64-mingw32"),
}

README = """Sid Meier's Colonization for Windows -- the native port ({target}, {version})

This is a port of the 1995 game's code to modern systems. It contains no game files:
you need your own copy of Colonization for Windows, the updated 1995 release (the one the
official patch produces; its COLONIZE.EXE has SHA-256 {patched}).

{how}

The game runs in your game folder, reads its pictures, sounds and data there, and saves
there, as the original did.

If it does not start or stops: every run writes colonization-port.log in this folder (the
launcher's steps, your system, and a crash's report), and a crash also leaves
colonization-port.dmp beside it. Please send both files with your report.
"""
HOW = {
    "exe": "Start Colonization.exe. The first time, it asks for the folder your copy is installed\n"
           "in (the one with COLONIZE.EXE), or unpack this kit into that folder.",
    "sh": "Needs Wine{extra}. Start it from a terminal:\n\n    ./colonization /path/to/your/Colonization/folder\n\n"
          "The folder is remembered; later just ./colonization.",
}
PATCHED = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit("%s failed:\n%s" % (" ".join(cmd), r.stderr[-1500:]))


def build(target, version):
    prog, name, kind, prefix = TARGETS[target]
    src = os.path.join(ROOT, prog)
    if not os.path.exists(src):
        return None
    top = "colonization-port-%s" % target
    stage = os.path.join(DIST, top)
    shutil.rmtree(stage, ignore_errors=True)
    os.makedirs(os.path.join(stage, "bin"))
    dst = os.path.join(stage, "bin", name)
    shutil.copyfile(src, dst)
    os.chmod(dst, 0o755)
    strip = (prefix + "-strip") if prefix else "strip"
    if shutil.which(strip):                       # the debug information stays in build/; best effort
        subprocess.run([strip, "--strip-debug", dst], capture_output=True)   # (macOS's strip has no ELF)
    if kind == "exe":
        run([prefix + "-gcc", "-O2", "-mwindows", "-DKIT_VERSION=\"%s\"" % version,
             "-o", os.path.join(stage, "Colonization.exe"), LAUNCHER_C,
             "-ladvapi32", "-lshell32", "-lole32"])
        how = HOW["exe"]
    else:
        shutil.copyfile(LAUNCHER_SH, os.path.join(stage, "colonization"))
        os.chmod(os.path.join(stage, "colonization"), 0o755)
        extra = (" with its 32-bit part (Debian/Ubuntu: dpkg --add-architecture i386; apt install wine wine32)"
                 if target == "linux-i386" else
                 " (Gcenx's wine-devel 11 from github.com/Gcenx/macOS_Wine_builds; not Apple's Game Porting Toolkit)")
        how = HOW["sh"].format(extra=extra)
        if target == "linux-i386":
            wrapper = os.path.join(os.path.dirname(src), "colonize.exe")
            if os.path.exists(wrapper):           # winegcc's loader stub beside the .so
                shutil.copyfile(wrapper, os.path.join(stage, "bin", "colonize.exe"))
                os.chmod(os.path.join(stage, "bin", "colonize.exe"), 0o755)
    with open(os.path.join(stage, "README.txt"), "w", newline="\r\n" if kind == "exe" else "\n") as f:
        f.write(README.format(target=target, version=version, patched=PATCHED, how=how))
    zpath = os.path.join(DIST, top + ".zip")
    with zipfile.ZipFile(zpath, "w", zipfile.ZIP_DEFLATED) as z:
        for base, _dirs, files in os.walk(stage):
            for n in sorted(files):
                p = os.path.join(base, n)
                info = zipfile.ZipInfo(os.path.relpath(p, DIST), time.localtime(os.path.getmtime(p))[:6])
                info.external_attr = (os.stat(p).st_mode & 0o777) << 16
                info.compress_type = zipfile.ZIP_DEFLATED
                with open(p, "rb") as fh:
                    z.writestr(info, fh.read())
    return zpath


def main(argv):
    version = argv[argv.index("--version") + 1] if "--version" in argv else \
        subprocess.run(["git", "-C", ROOT, "describe", "--always", "--dirty"], capture_output=True, text=True).stdout.strip()
    names = [a for a in argv if a in TARGETS] or list(TARGETS)
    os.makedirs(DIST, exist_ok=True)
    made = 0
    rec = {}
    for t in names:
        z = build(t, version)
        print("%-12s %s" % (t, "%s (%d KB)" % (os.path.relpath(z, ROOT), os.path.getsize(z) // 1024) if z else
                             "not built: %s missing" % TARGETS[t][0]))
        made += bool(z)
        rec[t] = {"built": bool(z), "kb": os.path.getsize(z) // 1024 if z else 0}
    if "--record" in argv:                          # status/kit.json: what tools/status.py's W10 row reads
        import json
        p = os.path.join(ROOT, "status", "kit.json")
        doc = json.load(open(p)) if os.path.exists(p) else {}
        doc.setdefault("targets", {}).update(rec)
        doc.setdefault("clean_machine", {})          # by hand: {"windows": {"by": .., "date": ..}, ...}
        with open(p, "w") as f:
            json.dump(doc, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if made == len(names) or not any(a in TARGETS for a in argv) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
