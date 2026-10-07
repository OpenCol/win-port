/* font_handle_1068_dd1e: candidate for 1068:dd1e (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The font reference's HANDLE: the word at +0.
 *
 * Every call site hands the result straight to `MSGetTextLength` (text_line_count, text_widest_line)
 * or `MSGetLineHeight` (ask_choice), all of which take a font handle, and the argument is
 * ask_choice's local Font set up by game_font_set_1068_de7b. `GameFont` is `{ short handle; short height; }`, so this is
 * its first field and `font_height_*` is its second.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: font_handle_1068_dd1e
 */

#include "colonization.h"

#include "classes.h"

int font_handle_1068_dd1e(Font *font)
{
    return font->hfont;
}
