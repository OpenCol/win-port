#!/usr/bin/env python3
"""A release's notes and checksums (roadmap W10), from the kits tools/kit.py made.

    python3 tools/release_notes.py VERSION      writes dist/RELEASE_NOTES.md and dist/SHA256SUMS.txt

Run by CI's kits job before `gh release create` (a v* tag, or the workflow run by hand with a
version: tools/release.sh). The figures come from status/*.json, as STATUS.md's do.
"""
import hashlib
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIST = os.path.join(ROOT, "dist")
KITS = [
    ("windows-x86", "Windows (32-bit program; runs on 32- and 64-bit Windows)", "`Colonization.exe`"),
    ("windows-x64", "Windows (64-bit program)", "`Colonization.exe`"),
    ("linux-i386", "Linux (a Winelib program: needs Wine with its 32-bit part)", "`./colonization`"),
    ("macos", "macOS (the 32-bit Windows program under Wine: needs Gcenx's Wine, not Apple's Game "
              "Porting Toolkit)", "`./colonization`"),
]
PATCHED = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
IMAGE = "assets/sailor-call-accross-sea.png"


def rec(name):
    p = os.path.join(ROOT, "status", name + ".json")
    return json.load(open(p)) if os.path.exists(p) else {}


def lockstep_line(target, label):
    sc = rec("lockstep" + ("" if target == "win32" else "-" + target)).get("scenarios", {})
    if not sc:
        return None
    same = sum(v["in_lockstep"] for v in sc.values())
    total = sum(v["checkpoints"] for v in sc.values())
    return "- %s: %d of %d checkpoints in lockstep with the original game over %d scenarios" % (
        label, same, total, len(sc))


def main(argv):
    if not argv:
        sys.exit(__doc__)
    version = argv[0]
    sums, rows = [], []
    for target, what, how in KITS:
        z = os.path.join(DIST, "colonization-port-%s.zip" % target)
        if not os.path.exists(z):
            continue
        h = hashlib.sha256(open(z, "rb").read()).hexdigest()
        sums.append("%s  %s" % (h, os.path.basename(z)))
        rows.append("| `%s` | %s | %s |" % (os.path.basename(z), what, how))
    if not rows:
        sys.exit("no kits in dist/: run tools/kit.py first")
    tested = [l for l in (lockstep_line("win32", "Windows x86 program"), lockstep_line("x64", "Windows x64 program"),
                          lockstep_line("linux", "Linux program"), lockstep_line("linux64", "Linux x86_64"))
              if l]
    sessions = rec("sessions")
    windows = rec("kit").get("clean_machine", {}).get("windows")
    # the README's picture, at this release's tag: a release page resolves no relative path (in a
    # private repository it shows to those who can see the repository)
    repo = os.environ.get("GITHUB_REPOSITORY", "OpenCol/win-port")
    picture = "![A sailor in the crow's nest points toward land](https://github.com/%s/raw/%s/%s)" % (repo, version, IMAGE)
    notes = picture + """

Sid Meier's Colonization for Windows (1995), ported to modern systems from its byte-matched
reconstruction. **No game files are included**: you need your own copy of the game -- either
**Colonization for Windows** (1995, patched or not), or **Steam's Colonization**, whose disc
image `COLONIZE.ISO` carries the Windows edition.

| Download | Platform | Start |
| --- | --- | --- |
{rows}

### How to run

1. Unzip the kit **into the folder of your game**:
   - Colonization for Windows: the folder with `COLONIZE.EXE` (for example `C:\\MPS\\COLONIZE`);
   - Steam's Colonization: its install folder, the one with `COLONIZE.ISO`
     (Steam: right-click the game > Manage > Browse local files).
2. Start the launcher from the kit's folder: `Colonization.exe` on Windows, `./colonization` in a
   terminal on macOS and Linux (install Wine first: on macOS `brew install --cask wine@devel`; on
   Debian/Ubuntu `sudo dpkg --add-architecture i386 && sudo apt install wine wine32`).

The launcher finds the game in the folder it was unzipped into (or one it is given: `Colonization.exe
"D:\\Games\\Colonization"`, `./colonization /path/to/game`), checks `COLONIZE.EXE`'s SHA-256, and
remembers the folder. Unzipped elsewhere, it looks in the usual places and then asks once.

- **Colonization for Windows**: the game runs in its own folder, reads its pictures, sounds and
  data there and saves there, as the original did.
- **Steam**: the launcher unpacks the Windows edition from `COLONIZE.ISO` once (a few seconds) into
  a folder of its own -- `%LOCALAPPDATA%\\colonization-port\\game` on Windows,
  `~/Library/Application Support/colonization-port/game` on macOS,
  `~/.local/share/colonization-port/game` on Linux -- and the game runs and saves there.

If it does not start or stops: `colonization-port.log` beside the launcher says why, and a crash
leaves `colonization-port.dmp` there too; please send both.

### How it was tested (status/*.json at this release)
{tested}
- The rules: the port plays the same saved games turn for turn with the original, compared
  at every step of every turn; the screens: {nsessions} scripted UI sessions.
{windows}
### Known limits
- A Steam copy (the original 1995 release) lacks the patch's `COLDATA9.DLL`: the Jukebox's
  Control Panel and the Cheat menu's dialogs crash. Everything else plays.
- The Windows game plays its music from the CD's audio tracks; a disc image has none.
- The macOS and Linux kits run through Wine; they are not native programs.

Version {version}. Checksums: `SHA256SUMS.txt`.
""".format(rows="\n".join(rows), tested="\n".join(tested) or "- (no records)", version=version,
           nsessions=len(sessions) or "the",
           windows=("- On a real Windows machine: %s, %s (%s).\n" % (windows.get("machine", "Windows").split(" (")[0],
                    windows.get("kit", ""), windows.get("date", "")) if windows else ""))
    open(os.path.join(DIST, "RELEASE_NOTES.md"), "w").write(notes)
    open(os.path.join(DIST, "SHA256SUMS.txt"), "w").write("\n".join(sums) + "\n")
    print(notes)


if __name__ == "__main__":
    main(sys.argv[1:])
