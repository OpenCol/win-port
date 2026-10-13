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

### W9 / W10: a real Windows x64 machine -- settled: no x64 release

Since v0.1.9 (Daniel, 2026-10-14) there is no 64-bit Windows kit: the 32-bit program runs on
64-bit Windows, and the x64 one had problems of its own there (v0.1.1's heap crash). The x64
program still builds (`PORT_TOOLCHAIN=mingw-x64`) and its lockstep records stand.

### W10: the pre-patch -> patched delta

The roadmap's kit offers Steam/GOG copies the 1995 patch as a binary delta
(col-win-re docs/findings/patch-diff.md). The port needs the patched `COLONIZE.EXE`: it reads
its initial data from it. A delta between the two builds carries about 52 KB of the patched
program's code (patched segment 16 and the relinked segments), which is distributing game
bytes -- a licensing question (rules 7 and 8). The kit's launcher detects a pre-patch copy
by its hash and says what is needed; it does not carry the delta.

To decide: may the kit carry the delta (or fetch the official patch from a stated place)?

Since 2026-10-12 the delta is needed for less: the port's tables and menus are compiled in, and
the launchers accept the original release (and unpack it from Steam's COLONIZE.ISO). What a
pre-patch copy still lacks is the patch's two new files, COLDATA9.DLL and COCKGUN.WAV -- for
which Daniel plans open (MIT) replacements -- not the patched program.

### W10: a clean machine per OS

The gate -- a clean machine of each OS, given only the kit and a game copy, reaches the map --
needs those machines.

- Windows: done with the x86 kit (a tester's Windows 7 SP1 64-bit, v0.1.1, 2026-10-11;
  status/kit.json). The x64 kit is no longer released (v0.1.9).
- Linux: the linux-i386 kit on a machine with only Wine (wine32) installed.
- macOS: the macos kit on a Mac with only Gcenx's Wine 11 installed (not the development Mac).
