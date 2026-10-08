# src/asm

Milestone W5. C replacements for the game's assembly: segments 17-19 of COLONIZE.EXE.
`tests/asm.py` holds each to the original (the gate; `status/asm.json`).

## Inventory

What fills the three segments, from col-win-re's relink map: the relink of the shipped program
(TLINK, the same object order and libraries) reproduces COLONIZE.EXE but one byte, so the map's
module boundaries are the shipped ones. Entry points are the far references into each segment
from the rest of the executable (its relocation records), plus the segments' own labels.

| Segment | Range       | Module                         | Entry points (all reached from outside)            | Here |
| ------- | ----------- | ------------------------------ | -------------------------------------------------- | ---- |
| 17 1080 | 0000-0205   | the game's `seg17.asm`         | 0000 `asm_decode_delta_flc`, 00e4 `asm_decode_byte_run`, 0168 `asm_decode_literal` (from 1070's MSDecode*); 01dc `L17_01dc`, 01f0 `L17_01f0`, 01fe `L17_01fe` (from 1068: random_below, the seed); 01ac, the generator, internal | `seg17_flic.c` |
| 18 1088 | 0000-03eb   | the game's `seg18.asm`         | 0000 `asm_lzw_decode` (from 1078's MSPicDecompress) | `asm_lzw_decode.c` |
| 19 1090 | 0000-047d   | the game's `seg19.asm`         | 0000 `asm_wipe_rows`, 00ff `asm_copy_bits`, 01b4 `asm_fill_rect`, 0215 `asm_draw_sprite`, 0348 `asm_draw_mono_sprite` (from 1068) | `seg19_blit.c` |
| 19 1090 | 047e-3bad   | Borland CWL.LIB `xxa`, `xxv`, `xx`, `xxas` | C++ exception handling: `__InitExceptBlock` (047e, every module's), `terminate`, `set_terminate`, `set_unexpected`, `_ThrowException`, `RaiseException`, `destroyVariable`, `destroyArray`, `__InitExceptBlocks`/`__ExitExceptBlocks`, `typeinfo` | the compiler's runtime |
| 19 1090 | 3bae-47e5   | Borland CWL.LIB `ctor2`, `ctor3`, `strings` | the `string` / `TStringRef` class (from the C runtime in 1000) | the compiler's runtime |

The "17 KB not yet described" of the roadmap is the last two rows: Borland's runtime, linked
after the game's segment-19 code into the same segment. No game source calls it (no `string`,
`terminate` or exception helper is named in src/); the compiler's own runtime does that work in
the port. So W5 is 12 entry points.

## Kept quirks and open points

- `asm_decode_literal` (1080:0168) moves the destination on by EDX after each row, a register
  it never sets: what its caller (MSDecodeLiteral, which sets nothing either) left there. The C
  moves it by stride - w. tests/asm.py runs the original with EDX = stride - w, so everything
  else is compared; no FLIC in the game has a chunk 16, so the game never reaches it.
- `asm_wipe_rows` reads and writes `ds:[row + ...]` / `es:[row + ...]`: its row tables are
  offsets in the bitmaps' segments and its far pointers' offsets are not added. The game's are
  0 (WinG DIBs), where the C's `src + rows[r]` is the same address.
- `asm_decode_byte_run` adds the row gap to the source as well as the destination; kept.
- Counts of 0 run 65536 times in the original's do-while loops; kept.
