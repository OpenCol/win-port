# BLOCKED

Items set aside under roadmap rule 6 (one function or one divergence that survived about 25
attempts) and rule 7 (a question for a person: visual judgement, licensing, a section 1
decision, a machine this one cannot be). Each entry gives the milestone, what it is, the
evidence, what was tried, and what would settle it.

## For a person

### W9: Linux x86_64 as native Winelib, or the x64 Windows program under Wine?

The roadmap names Winelib for Linux and macOS (section 1). A 64-bit Winelib build is LP64:
`long` is 8 bytes there, and the game's `long` is its 32-bit integer everywhere (thousands of
uses in the imported corpus, saves and checkpoints laid out by it). MinGW x64 is LLP64 --
`long` stays 4 bytes -- which is why the 64-bit work went into the Windows x64 program.

What is measured now: Linux x86_64 runs the x64 Windows program under the Linux machine's
64-bit Wine (`PORT_TARGET=linux64`), and macOS runs it under Wine 11 (`PORT_TARGET=x64`).
Both give the Windows build's lockstep results (status/lockstep-x64.json,
status/lockstep-linux64.json).

To decide: is "64-bit Linux" satisfied by the x64 program under Wine, or must it be a native
LP64 Winelib build (then every game `long` would have to become `int32_t` -- a corpus-wide
retype of the imported files' spellings)?

### W9 / W10: a real Windows x64 machine

The x64 program has only run under Wine (macOS, Linux). It needs one run on 64-bit Windows:
`build/x64/colonize.exe` in a copy of the game directory, start a new game, load a save.

### W10: the pre-patch -> patched delta

The roadmap's kit offers Steam/GOG copies the 1995 patch as a binary delta
(col-win-re docs/findings/patch-diff.md). The port needs the patched `COLONIZE.EXE`: it reads
its initial data from it. A delta between the two builds carries about 52 KB of the patched
program's code (patched segment 16 and the relinked segments), which is distributing game
bytes -- a licensing question (rules 7 and 8). The kit's launcher detects a pre-patch copy
by its hash and says what is needed; it does not carry the delta.

To decide: may the kit carry the delta (or fetch the official patch from a stated place)?

### W10: a clean machine per OS

The gate -- a clean machine of each OS, given only the kit and a game copy, reaches the map --
needs those machines.
