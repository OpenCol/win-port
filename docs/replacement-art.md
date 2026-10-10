# Replacement art: the 22 pictures still taken from COLONIZE.EXE

The port reads everything the game draws from the game's data files (`COLDATA*.DLL`,
`COLTEXT0.DLL`, the WAVs), except 22 small pictures that are resources of `COLONIZE.EXE`
itself: 12 cursors, 4 icons and 6 bitmaps. Its tables, menus and dialog are compiled in
(`src/gen/exe_data.c`); with new versions of these pictures the port would no longer need
`COLONIZE.EXE`. Until then, a run without the file plays, without these pictures
(`PORT_NO_EXE=1 python3 tests/sessions.py`).

`python3 tools/exe_resources.py /path/to/COLONIZE.EXE OUT` extracts the originals from your
own copy, as `.cur`, `.ico`, `.bmp` and `.png`, with `sheet.png` showing all of them at 4x.
They are never committed (roadmap rule 8).

## What a replacement must be

The same id, size, colour depth and hotspot, so that it is a drop-in replacement.

| File | Size | Colours | Hotspot | What it is | Where the game uses it |
| --- | --- | --- | --- | --- | --- |
| cursor-128.cur | 32x32 | 2 (+ transparent) | 0,0 | arrow pointing up-left, with the words "Go to" | map: dragging from the selected unit to another square |
| cursor-129.cur | 32x32 | 2 | 6,5 | arrow pointing north | map: the pointer over the top ninth of the selected unit's square (move that way) |
| cursor-130.cur | 32x32 | 2 | 8,3 | arrow pointing north-east | likewise, top-right |
| cursor-131.cur | 32x32 | 2 | 8,6 | arrow pointing east | right |
| cursor-132.cur | 32x32 | 2 | 8,8 | arrow pointing south-east | bottom-right |
| cursor-133.cur | 32x32 | 2 | 6,8 | arrow pointing south | bottom |
| cursor-134.cur | 32x32 | 2 | 4,7 | arrow pointing south-west | bottom-left |
| cursor-135.cur | 32x32 | 2 | 5,6 | arrow pointing west | left |
| cursor-136.cur | 32x32 | 2 | 4,4 | arrow pointing north-west | top-left |
| cursor-137.cur | 32x32 | 2 | 0,0 | a large arrow pointer | loaded at start; no use found in the code |
| cursor-138.cur | 32x32 | 2 | 7,7 | a crosshair | the minimap window |
| cursor-139.cur | 32x32 | 2 | 4,7 | an hourglass | loaded at start; no use found in the code |
| icon-128.ico | 32x32 | 256 | | an American flag in a wooden frame | the game itself; main and map windows |
| icon-129.ico | 32x32 | 256 | | a man (a colonist) in a wooden frame | status window |
| icon-130.ico | 32x32 | 256 | | a red book in a frame | Colonopedia |
| icon-131.ico | 32x32 | 256 | | a sailing ship in a frame | reports and minimap windows |
| bitmap-301.bmp | 32x32 | 16 | | radio button, filled grey, on teal | option dialogs' radio buttons |
| bitmap-302.bmp | 32x32 | 16 | | radio button with a black dot, on teal | " |
| bitmap-303.bmp | 32x32 | 16 | | radio button, an empty ring, on teal | " |
| bitmap-304.bmp | 32x32 | 16 | | check box with a tick, on teal | option dialogs' check boxes |
| bitmap-305.bmp | 32x32 | 16 | | check box, filled dark grey, on teal | " |
| bitmap-306.bmp | 32x32 | 16 | | check box, filled light grey, on teal | " |

Notes for an artist:

- **Cursors** are two-colour (black and white) plus transparent, the Windows 3.1 kind. The
  hotspot is the pixel that points; the arrows' hotspots sit at their tips' side of the
  picture. The eight direction arrows should read as a set.
- **Icons** are 256-colour, 32x32. A modern .ico may carry larger sizes too; the game asks for
  32x32.
- **Bitmaps** are drawn as they are, with no transparency, from their top-left corner and cut
  to the button's size (`bitmap_button_proc`: BitBlt SRCCOPY). Keep the picture where the
  original has it and keep its teal ground (RGB 0,128,128), which shows.
  Which bitmap shows for which state is chosen when the button is painted (enabled, hot,
  disabled); the descriptions above say what each looks like, not what it means.

Ids, sizes and hotspots are from `tools/exe_resources.py` run on the patched 1995 release;
the uses are from the code (`game_init`: cursors 0x80-0x8b; `MapWin_TrackMouse` and
`MapWin_WinPos2MapPos`: the direction ninths; `MiniWin_Init`: cursor 138; the `win_set_icon`
callers: icons; `MSInitRadio`, `MSInitCheck`: bitmaps).
