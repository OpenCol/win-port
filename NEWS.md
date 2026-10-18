# What's new

Each release's entry here heads its release notes on GitHub (tools/release_notes.py takes the
section for the version being released).

## v0.1.11

- **Wide screens and zooming out** (#7): a map window larger than the map -- a 2560- or
  3440-pixel-wide screen, or zoomed out on a smaller one -- showed a black band on the left and
  drew discovered land as ocean next to its units. The map now starts at the window's left and
  top edge, with black beyond its east and south edges, and every square is drawn where it is.
  The 1995 game had the same flaw, but no screen of its day was large enough to show it.

## v0.1.10

- **Saving works on Windows 7** (testers: the 32-bit Save button did nothing there, while the
  64-bit program, and the 32-bit one on Windows 10 and 11, saved). Three changes:
  - Windows 7 and 8 get the open/save dialog of Windows XP instead of Vista's shell dialog,
    the one that failed (`PORT_XP_DIALOG=1` or `0` in the environment overrides the choice).
  - The game's 64 MB stack is now its own thread's, not every thread's. Windows gives each new
    thread the program's stack size, and a 32-bit program has room for only about 29 threads of
    64 MB: too few for the shell dialog's own.
  - As in 1995 on Windows 3.1, the dialog suggests the save's name (`eng1492.sav`) in its name
    box and lists the `*.sav` files, and the name keeps the game's folder.
- `colonize-port.log` says more about each dialog: which kind it was, the room the program had
  (address space, threads), and what it returned.

## v0.1.9

- **One Windows kit: `colonization-port-windows-x86`.** The 64-bit Windows kit is no longer
  released. The 32-bit program runs on both 32- and 64-bit Windows, and is the one testers found
  working; players of the x64 kit should switch to it -- saved games carry over unchanged.

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
