/* create_game_fonts: candidate for 1050:982a (73 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STCON_()
 * name-source: descriptive
 *
 * Constructs module 1050's two file-scope fonts -- Arial at 16 and Arial at 12 -- and
 * `destroy_game_fonts` at the next address releases them in reverse.
 *
 * THE ARGUMENTS ARE TWO INTS, AND THIS IS WHERE THAT IS PROVED. `Font_ctor_1050_7f9b`
 * hands `[bp+0xc]` and `[bp+0xa]` straight to `MSGetTypeFont`, and a four-byte argument
 * passed through untouched reads equally well as one pointer or as two `int`s -- the
 * callee's bytes cannot tell them apart. Here the caller pushes `0x10` and `0x1` as plain
 * literals with no relocation against either, which a pointer could not be, and
 * `MSGetTypeFont(int kind, int height)` was already reconstructed with that signature:
 * kind 1 is Arial. So the constructor is `Font(int, int)`, and its header has been
 * corrected.
 *
 * NOTHING IN THE SOURCE IS THIS FUNCTION: it is `_STCON_`, written by Borland from two
 * file-scope definitions, which is why this file has no function in it.
 *
 * These two are NEAR, not -- the shipped code pushes `DS` where the map ports at
 * 1040:4542 push a segment -- so they live in the data group with the rest of the module's
 * statics.
 * Font::Font is defined in-file first, so the static constructor reaches it by the 4-byte call.
 *
 * The destructor is defined in-file as well, for `destroy_game_fonts`'s call (the same text).
 */

#include "colonization.h"

#include "classes.h"

/* PORT: an in-file stub of Font::Font removed: src/game/Font_ctor_1008_d9c4.c defines it (W2). */
/* PORT: an in-file stub of Font::~Font removed: src/game/Font_dtor_1008_d4b5.c defines it (W2). */

Font g_font16(1, 0x10);     /* DGROUP:0x24e8 */
Font g_font12(1, 0xc);      /* DGROUP:0x24ec */
