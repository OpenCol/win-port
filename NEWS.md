# What's new

Each release's entry here heads its release notes on GitHub (tools/release_notes.py takes the
section for the version being released).

## v0.1.14

- **Check boxes keep what you set** (#9): unticking a box in the Custom House, or Fast Piece
  Slide in the options, looked fine until the window closed, and then nothing had changed. The
  port read every box after the first one from the wrong place in memory. It now reads each box
  from its own. Thanks to Ross Drew.
- **Units slide at a speed you can watch** (#6): the 1995 game moved a unit across a square as
  fast as the computer could draw it. That was slow enough to follow on a 1995 PC but looks like
  teleporting today. Each step of the slide now waits 12 ms, so a normal move takes about a fifth
  of a second, and Fast Piece Slide is still twice as fast. Set `PORT_SLIDE_MS` to change the
  wait, or to 0 to turn it off. Thanks to Ross Drew.
- **No more free horses and muskets** (#10): the colonist profession menu could hand out horses,
  muskets and other goods the colony didn't have. Part of the list of goods a job needs wasn't
  cleared first, and leftover values were added to the colony's stock. It's now cleared in full,
  as in the original. Thanks to Ross Drew.

## v0.1.13

- **New World maps** (#5): a generated map put mountains on nearly every square next to the
  sea. One step of the generator reads the memory just past the four world options. The
  original always finds 0 there, so the step runs 800 times. The port read a different value
  from that spot and ran the step millions of times. It now runs 800 times, like the original,
  and the land is mostly flat again. America's map was never affected.
- When the game stops responding, the log now says where it stopped. Windows marks a window
  "Not Responding" after five seconds without a message, and closing it then ended the game with
  0xCFFFFFFF and nothing in the log, which the launcher called "a crash". Now the stalled code's
  stack goes into the log as `hang` lines, a `colonize-port-hang.dmp` is saved next to it, and
  the launcher calls that exit a hang. The log's header also lists the CD drives: the music is
  CD audio, and the game waits on the drive.

## v0.1.12

- The port now keeps everything in its own folder. A Steam or GOG game it unpacks goes into a
  `game` folder next to `Colonization.exe` (or the `colonization` script), together with your
  saves, the folder it remembers and the log. Nothing is written to your user profile any more
  (`%LOCALAPPDATA%`, `%APPDATA%`, `~/Library/Application Support`, `~/.local/share`,
  `~/.config`), so the whole thing can live on a USB stick. To update, unzip the new kit over
  the old one and your `game` folder and saves stay.
- Coming from an older kit: the first start unpacks the game again into the new place and copies
  your saves over from the old one.
- Saving and loading work in deep folders too. Windows' old file functions refuse a path of 128
  characters or more, and a game inside Steam's own folder gets close to that.

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
