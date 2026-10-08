# win-port

A native port of **Sid Meier's Colonization for Windows** (MicroProse, 1995) is the goal here:
the same game, compiled with modern tools and running without an emulator, natively on Windows
and on Linux and macOS through Winelib. It is at the start. [STATUS.md](STATUS.md) says how far
it has got; nothing playable exists yet.

It is built from the byte-matched reconstruction of the original program
([col-win-re](https://github.com/dderevjanik/col-win-re), published as
[win-decomp](https://github.com/OpenCol/win-decomp)). That reconstruction compiles to the
shipped bytes, so it says exactly what the game does. This repository turns it into a program
modern compilers accept.

## The contract

**The port must play exactly like the patched 1995 `COLONIZE.EXE`.** It does not have to produce
the same bytes. Modern compilers can't, and that is why this repository exists apart from the
reconstruction. "Plays exactly like" is not a feeling. It is measured by gates, each a command
that exits 0:

- the per-function differential against the original code (W3);
- the autoplay lockstep: the original and the port, same seed, compared at every turn boundary
  of a corpus of recorded games (W6);
- scripted UI sessions and saves that load in both directions (W7).

[STATUS.md](STATUS.md) shows where each milestone stands. It is generated from what the gates
recorded and never written by hand.

| Milestone | |
| --- | --- |
| W0 | bootstrap: this repository, CI, the pinned import |
| W1 | every file compiles (MinGW i686 and winegcc) |
| W2 | it links |
| W3 | the oracle: the original program run in an emulator, before anything native runs |
| W4 | the platform layer, Win16 to Win32 |
| W5 | the assembly segments rewritten in C |
| W6 | the rules, in lockstep with the original |
| W7 | playable |
| W8–W10 | Linux native, 64-bit, a release kit |

The roadmap, with every decision and gate, is col-win-re's
[docs/reconstruction/win-port-roadmap.md](https://github.com/dderevjanik/col-win-re/blob/508d4d0d0c12b856a7f33534ae79c5af48c14939/docs/reconstruction/win-port-roadmap.md).

## Building and playing

The port needs the patched 1995 game files (`$COL_GAME_DIR`, hash-checked: see Game files), a
MinGW i686 toolchain and Wine.

```sh
export COL_GAME_DIR=/path/to/the/patched/game
python3 tools/undefined.py              # build: compile and link build/colonize.exe
python3 tools/run_port.py --play        # play it, under Wine
```

`--play` runs the game in a copy of the game directory kept at `build/play/game`, so your saves
stay between runs and the original directory is never written. Without `--play`, `run_port.py`
is the test runner: a fresh copy each run, a time limit, a log in `build/run/`.

The gates that measure the port are listed in `STATUS.md` (generated: `python3 tools/status.py`).

`toolchains/` also holds `clang-cl-x86` (the MSVC-compatible check), and `winegcc-i386` and
`winegcc-x86_64` for Linux and macOS (W8, W9).

## Where the code comes from

`src/game`, `src/gfx` and `src/platform` start as an import from col-win-re at the commit in
[import/SOURCE](import/SOURCE): each byte-matched function goes through col-win-re's
`tools/port_transform.py`. That step removes the standalone files' stubs and local
declarations, and strips Borland's `far`/`near`/`pascal`.

```sh
python3 import/import.py --check     # the tree is a fresh import, apart from marked PORT: edits
```

After the import, files are edited here as the port needs. Every edit to a function body
carries a `PORT:` comment saying what changed and why. `src/host` and `src/asm` are this
repository's own.

## Game files

None are in this repository, and none ever will be. The tests read your own patched install from
`$COL_GAME_DIR`, check its hashes, and refuse anything else. The Windows edition is not sold on
Steam or GOG, which sell the DOS version.

*Sid Meier's Colonization* is © 1994–1995 MicroProse Software, Inc. This project is not
affiliated with or endorsed by MicroProse, Take-Two Interactive, Firaxis Games or Sid Meier.
