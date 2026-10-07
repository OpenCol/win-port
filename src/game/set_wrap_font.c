/* set_wrap_font: candidate for 1040:605f (22 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the font `ParseFancyTextAndShow` lays its lines out in.
 *
 * All seven callers pass a font's ADDRESS -- among them `&g_font_small` in
 * `get_game_options`, `&g_number_font` in `PediaWin_DoUnitArticle`,
 * `&g_heading_font` in `PediaWin_DoFathersArticle` -- and `ParseFancyTextAndShow` is the only
 * reader, taking its height and handing it to `GRPort_String`. This file used to
 * spell the parameter `long` and note that `ParseFancyTextAndShow` spells it `void *`, leaving the
 * disagreement standing. The callers settle it: they pass an ADDRESS. Both spellings are the
 * same four bytes and both grade, which is exactly why the bytes could not decide it and the
 * call sites had to.
 */

#include "colonization.h"

void set_wrap_font(void *font)
{
    g_wrap_font = font;
}
