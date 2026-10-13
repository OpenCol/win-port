# What's new

Each release's entry here heads its release notes on GitHub (tools/release_notes.py takes the
section for the version being released).

## v0.1.8

- **The Save Game dialog** opens in the game's folder (where Win16's did, and where the game
  keeps its saves), and each open or save dialog writes a line to `colonize-port.log`: its
  folder, the name chosen or that it was closed, and any error. A tester's 32-bit Save button
  did nothing on Windows while the 64-bit one worked; should the program's thread be in a
  multithreaded COM apartment, which the shell's dialog does not support, the dialog now runs on
  a thread of its own. Not reproduced here (Wine's dialog is not Windows'): the log line says
  which case a player hit.

## v0.1.7

- **GOG's Colonization works too.** GOG's DOS release keeps the same disc as Steam's, as
  `COLONIZE.GOG` (a raw CD image): the launcher unpacks its Windows edition once, as it does
  Steam's `COLONIZE.ISO`. Both give the same game folder, byte for byte.
- **The launcher looks in a new order:** the current folder, then the launcher's own folder and
  the one above it, then the folder used last time -- in each, an installed Colonization for
  Windows first, then `COLONIZE.ISO`, then `COLONIZE.GOG`. Failing those it asks with a folder
  dialog (also on macOS and Linux now), and asks again until the chosen folder holds the game.
  An installed copy now needs its data files beside `COLONIZE.EXE`, not the program alone.

## v0.1.6

- **Water colour cycling.** River currents and beach surf shimmer while the map waits for your
  orders, as in the DOS version. The 1995 Windows game reserved the colours for it and kept the
  Game Options item "Water Color Cycling", but never animated them; this is win-patches'
  water-cycling, brought to the port. On by default; Game Options > Water Color Cycling turns
  it off (the setting is saved with the game). The game's rules are unchanged.
- For developers: `tools/coverage.py` (which game functions the tests run), a colony screen
  session, every UI session's screens checked against a baseline pixel for pixel, and
  `tools/gate.py`, one command for all of it; `tools/run_port.py --play` no longer stops on
  Python 3.14.
