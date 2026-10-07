/* destroy_log_window: candidate for 1060:b13b (40 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STDES_()
 * name-source: descriptive
 *
 * Destroys module 1060's two file-scope objects in reverse order: the Font at DGROUP:0x4e68
 * first, then the LogWindow at DGROUP:0x2ac0 that `GRPort_CopyToPort` writes its warnings to.
 * `create_log_window` at the previous address is the other half.
 *
 * This is `_STDES_`, the translation unit's static destructor, and it carries no exception
 * block -- there is nothing to unwind on the way out. The `2` is Borland's destructor flag
 * word: destroy the object, do not free its storage.
 *
 * Both objects are NEAR: the shipped code pushes `DS` and an offset, not a segment.
 *
 * Both destructors are reached by the four-byte `PUSH CS / CALL NEAR` form, so both are in
 * this translation unit and appear here as stubs (rule 47).
 *
 * THIS FILE IS THE CONSTRUCTOR'S TEXT WITH THE SYMBOL CHANGED (2026-09-29), as
 * destroy_game_globals is: Borland writes `_STCON_` and `_STDES_` from the same file-scope
 * definitions, and a merged module (tools/merge_module.py) carries those definitions once and
 * lets the compiler place both. The plain-C spelling this replaced called each destructor by
 * hand, and would have been a second copy beside the compiler's.
 */

#include "colonization.h"

#include "classes.h"

Font::Font()
{
}

Font::~Font()
{
}

LogWindow g_error_log(g_log_name);        /* DGROUP:0x2ac0 */
Font g_log_font;                        /* DGROUP:0x4e68 */
