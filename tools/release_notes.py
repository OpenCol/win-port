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
    ("windows-x86", "Windows (32-bit program; runs on 32- and 64-bit Windows)",
     "Unzip, start `Colonization.exe`; the first time it asks for your game folder."),
    ("windows-x64", "Windows (64-bit program)",
     "Unzip, start `Colonization.exe`; the first time it asks for your game folder."),
    ("linux-i386", "Linux (native Winelib program; needs Wine with its 32-bit part)",
     "Unzip, run `./colonization /path/to/your/game/folder`."),
    ("macos", "macOS (the Windows program under Wine)",
     "Unzip, run `./colonization /path/to/your/game/folder`; needs a current Wine "
     "(Gcenx's wine-devel 11, not Apple's Game Porting Toolkit)."),
]
PATCHED = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"


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
    notes = """Sid Meier's Colonization for Windows (1995), ported to modern systems from its byte-matched
reconstruction. **No game files are included**: you need your own copy of Colonization for
Windows, the updated 1995 release (its `COLONIZE.EXE` has SHA-256 `{patched}`). The
launcher checks this; the original 1995 release (before the official patch) is recognised and
refused.

| Download | Platform | How |
| --- | --- | --- |
{rows}

The game runs in your game folder: it reads its pictures, sounds and data there and saves
there, as the original did. Do not copy `bin/colonize.exe` into the game folder: Windows does
not tell its name from the game's own `COLONIZE.EXE`.

### How it was tested (status/*.json at this release)
{tested}
- The rules: the port plays the same saved games turn for turn with the original, compared
  at every step of every turn; the screens: 9 scripted UI sessions on every target.

### Known limits
- The programs have run on macOS and Linux (under Wine), but not yet on a real Windows machine.
- The kit does not carry the 1995 patch for an unpatched copy.

Version {version}. Checksums: `SHA256SUMS.txt`.
""".format(patched=PATCHED, rows="\n".join(rows), tested="\n".join(tested) or "- (no records)", version=version)
    open(os.path.join(DIST, "RELEASE_NOTES.md"), "w").write(notes)
    open(os.path.join(DIST, "SHA256SUMS.txt"), "w").write("\n".join(sums) + "\n")
    print(notes)


if __name__ == "__main__":
    main(sys.argv[1:])
