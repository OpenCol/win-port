/* MSGetLineHeight_1050_834b: candidate for 1050:834b (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 32. The last 8 are a second, unreachable epilogue, the same shape as
 * `IsBackdrop` has.
 *
 * Forwards the object's first word -- a font handle -- to `MSGetLineHeight` and
 * returns what that gives.
 *
 * `Popup_StringPourCalculate` already says so from the outside: "the line height comes from
 * `1050:834b` and the space width from measuring \" \"". `Popup_button_x_size` measures with
 * it "off the text pointer at +0x6e", which is the same word seen through a different typedef.
 *
 * THE OBJECT IS THE GAME'S Font (2026-10-01), not the platform's FontRec: Popup::font at +0x6e
 * holds g_popup_font, which is &g_font16 or a font Popup_set_font installs, and both are
 * `Font` objects (hfont, height) built by create_game_fonts. The platform keeps its own
 * FontRec (hfont, owned) behind the handle this forwards.
 *
 * RENAMED 2026-09-27 from ctl_line_height: Wrapper of MSGetLineHeight (1078:140e), a singleton. Callers: Popup_StringPourCalculate, button_y_size, draw, edit_y_size, item_y_size and set_list, which are Mac MSGetLineHeight's 6 Popup callers exactly. mac_bind proposed the bare name; it goes to the body. Font wrappers pass the font record's handle at +0, the shape of MSEnableItem_1048_e2dd (the menu handle at +0). They take MSName_<seg>_<off>, and the 1078 body takes the bare Mac name.
 */

#include "colonization.h"

extern int MSGetLineHeight(unsigned h);

#include "classes.h"
int MSGetLineHeight_1050_834b(Font *font)
{
    return MSGetLineHeight(font->hfont);
}
