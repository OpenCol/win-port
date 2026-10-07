/* game_font_set_1068_de7b: candidate for 1068:de7b (78 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Re-points a 4-byte GameFont slot at a different typeface: release whatever handle is
 * already there, ask `MSGetTypeFont(kind, height)` for a new one, and cache its line height
 * from `MSGetTextHeight` at +2. It is `Font_ctor_1008_d9c4`'s body with a release in front,
 * so it is the assignment the constructor is not.
 *
 * TRANSFERRED: byte-identical to `game_font_set_1008_cfe5` (1008:cfe5); this is the graphics
 * segment's copy. The
 * suffix is what tells them apart, as it does for Font_ctor_1008_d9c4 and _1050_7f9b.
 *
 * The record is the one that constructor establishes -- `short handle; short height;` -- and
 * the two `int` parameters are the same pair it passes straight to `MSGetTypeFont`.
 *
 * Written with an explicit leading pointer rather than as a member function: under `-ml` a
 * `this` occupies the first parameter slot at `[BP+6]`, which is where this reads it, so
 * the two spellings compile alike and the C one does not claim a mangled symbol the image
 * has not shown.
 */

#include "colonization.h"

#include "classes.h"

extern int MSGetTextHeight(unsigned hfont);

void game_font_set_1068_de7b(Font *font, int kind, int height)
{
    if (font->hfont != 0)
        MSReleaseFont(font->hfont);
    font->hfont = MSGetTypeFont(kind, height);
    font->height = MSGetTextHeight(font->hfont);
}
