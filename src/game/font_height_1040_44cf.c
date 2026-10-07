/* font_height_1040_44cf: candidate for 1040:44cf (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The font's LINE HEIGHT: the `int` at +0x2. A one-line accessor, 20-odd bytes of
 * prologue and epilogue around a single move.
 *
 * Named from what the callers do with it: `y += font_height(...)`, `y0 += ...`, `th = ...`,
 * `rh = ...` -- it advances a text cursor by one line. The argument is always a `g_font_*`
 * reference or a `Font *` (the game's Font object, `{ short hfont; short height; }`), so
 * this is its second field and `font_handle_*` its first.
 */

#include "colonization.h"

#include "classes.h"

int font_height_1040_44cf(Font *font)
{
    return font->height;
}
