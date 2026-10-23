#!/usr/bin/env python3
"""The release kit (roadmap W10): one zip per platform, the port and a launcher, no game files.

    python3 tools/kit.py [TARGET ...] [--version V] [--record]     targets: windows-x86 linux-i386 macos
                                                          (default: every target whose program is built)

    windows-x86   Colonization.exe (the launcher) + bin/colonize.exe       (tools/undefined.py)
    linux-i386    colonization (the launcher script) + bin/colonize.exe.so  (PORT_TOOLCHAIN=winegcc, Winelib)
    macos         colonization + bin/colonize.exe, run under Wine           (the Windows x86 program)

The launchers (src/launcher) find the player's copy of the game, check COLONIZE.EXE's SHA-256
against the updated 1995 release, and start the port in that folder. Writes dist/*.zip.

No 64-bit Windows kit since v0.1.9 (Daniel, 2026-10-14): the 32-bit program runs on 32- and
64-bit Windows, and the 64-bit one had problems of its own on real Windows the 32-bit did not.
PORT_TOOLCHAIN=mingw-x64 still builds build/x64/colonize.exe for testing.
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
ISOIMPORT_C = os.path.join(ROOT, "src", "launcher", "isoimport.c")

TARGETS = {
    # target: (program, its name in bin/, launcher kind, mingw prefix for the launcher/strip)
    "windows-x86": ("build/colonize.exe", "colonize.exe", "exe", "i686-w64-mingw32"),
    "linux-i386": ("build/linux/colonize.exe.so", "colonize.exe.so", "sh", None),
    "macos": ("build/colonize.exe", "colonize.exe", "sh", "i686-w64-mingw32"),
}

README = """Sid Meier's Colonization for Windows -- the port ({target}, {version})

This is a port of the 1995 game's code to modern systems. It contains no game files:
you need your own copy -- Colonization for Windows (1995, patched or not; the patched
COLONIZE.EXE has SHA-256 {patched}), or Steam's or GOG's Colonization.

{how}

Everything this kit writes stays in this folder: a Steam or GOG game it unpacked (the game
folder) with your saves in it, the folder it remembers (gamedir), and the log. Nothing goes
into your user profile. To update, unzip the new kit over this one: the game folder and your
saves stay. With an installed Colonization for Windows the game runs in its own folder and
saves there, as the original did.

If it does not start or stops: every run writes colonization-port.log in this folder (the
launcher's steps, your system, and a crash's report), and a crash also leaves
colonization-port.dmp beside it. Please send both files with your report.
"""
HOW = {
    "exe": "Start Colonization.exe. The first time, it asks for the folder your copy is installed\n"
           "in (the one with COLONIZE.EXE), or unpack this kit into that folder.\n\n"
           "Steam's or GOG's Colonization works too: choose its folder (the one with COLONIZE.ISO or\n"
           "COLONIZE.GOG), and the "
           "launcher unpacks the Windows edition from the disc image once, into the game\n"
           "folder beside Colonization.exe, and plays from there.",
    "sh": "This is not a native {system} program: it runs through Wine, which must be installed\n"
          "first{extra}\n\n"
          "Start it from a terminal, the first time with the folder your copy is installed in (the\n"
          "one with COLONIZE.EXE):\n\n    ./colonization /path/to/your/Colonization/folder\n\n"
          "The folder is remembered; later just ./colonization. Steam's or GOG's Colonization works\n"
          "too: give its folder (the one with COLONIZE.ISO or COLONIZE.GOG), and the script unpacks\n"
          "the Windows edition from the "
          "disc image once (into the game folder beside this script) and plays from there.\n\n"
          "The script uses $PORT_WINE if set,\n"
          "else `wine` on PATH, else Gcenx's Wine app in /Applications, else wine64.\n"
          "$PORT_DESKTOP sets the game's window size (default 1024x768).",
}
PATCHED = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
README_NAME = "README-colonization-port.txt"


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
    def launcher(cc_prefix, out):
        run([cc_prefix + "-gcc", "-O2", "-mwindows", "-DKIT_VERSION=\"%s\"" % version, "-o", out,
             LAUNCHER_C, ISOIMPORT_C, "-ladvapi32", "-lshell32", "-lole32"])

    if kind == "exe":
        launcher(prefix, os.path.join(stage, "Colonization.exe"))
        how = HOW["exe"]
    else:
        shutil.copyfile(LAUNCHER_SH, os.path.join(stage, "colonization"))
        # the Windows launcher, for its --import only: the script unpacks Steam's COLONIZE.ISO
        # with it under Wine (i686, which every Wine runs)
        launcher("i686-w64-mingw32", os.path.join(stage, "bin", "colonization-import.exe"))
        os.chmod(os.path.join(stage, "colonization"), 0o755)
        if target == "linux-i386":
            system, extra = "Linux", (
                ", with its 32-bit part. On Debian or Ubuntu:\n\n"
                "    sudo dpkg --add-architecture i386 && sudo apt update\n"
                "    sudo apt install wine wine32\n\n"
                "The port here is a Winelib program: compiled on Linux against Wine's libraries,\n"
                "and started by Wine's loader (bin/colonize.exe beside bin/colonize.exe.so).")
        else:
            system, extra = "macOS", (
                ". Use a current Wine -- Gcenx's builds:\n\n"
                "    brew install --cask wine@devel\n\n"
                "(or from github.com/Gcenx/macOS_Wine_builds), which puts Wine Devel.app in\n"
                "/Applications. Not Apple's Game Porting Toolkit (Wine 7.7, often Homebrew's wine64):\n"
                "it stops the game the first time a sound plays.\n\n"
                "The port here is the 32-bit Windows program, run by Wine as any Windows program is;\n"
                "on Apple silicon, Rosetta 2 translates its x86 code.")
        how = HOW["sh"].format(system=system, extra=extra)
        if target == "linux-i386":
            wrapper = os.path.join(os.path.dirname(src), "colonize.exe")
            if os.path.exists(wrapper):           # winegcc's loader stub beside the .so
                shutil.copyfile(wrapper, os.path.join(stage, "bin", "colonize.exe"))
                os.chmod(os.path.join(stage, "bin", "colonize.exe"), 0o755)
    # not README.txt: the kit is meant to be unpacked into the game folder, whose own README.TXT
    # (MicroProse's, or the 1995 patch's) it would replace -- Windows names ignore case
    with open(os.path.join(stage, README_NAME), "w", newline="\r\n" if kind == "exe" else "\n") as f:
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
