/* destroy_game_fonts: candidate for 1050:9873 (40 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STDES_()
 * name-source: descriptive
 *
 * Releases module 1050's two file-scope fonts in reverse order, the 12-point at
 * DGROUP:0x24ec before the 16-point at DGROUP:0x24e8. `create_game_fonts` at the
 * previous address builds them.
 *
 * This is `_STDES_`, the translation unit's static destructor, and it carries no exception
 * block -- there is nothing to unwind on the way out. The `2` is Borland's destructor flag
 * word: destroy the object, do not free its storage.
 *
 * Both objects are NEAR: the shipped code pushes `DS` and an offset, not a segment. The
 * destructor is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 *
 * THIS FILE IS THE CONSTRUCTOR'S TEXT WITH THE SYMBOL CHANGED (2026-09-29), as
 * destroy_game_globals is: Borland writes `_STCON_` and `_STDES_` from the same file-scope
 * definitions, and a merged module (tools/merge_module.py) carries those definitions once and
 * lets the compiler place both. The plain-C spelling this replaced called each destructor by
 * hand, and would have been a second copy beside the compiler's.
 */

#include "colonization.h"

#include "classes.h"

Font::Font(int kind, int height)
{
}
Font::~Font()
{
}

Font g_font16(1, 0x10);     /* DGROUP:0x24e8 */
Font g_font12(1, 0xc);      /* DGROUP:0x24ec */
