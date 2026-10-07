/* PieceIconDraw: candidate for 1040:1fcc (140 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * draw_sprite_with_shadow with the record chosen by index from the third 32-byte sprite
 * table: with bit 0 a flat copy two pixels to the left (x - 2) in colour 0x5f when bit 2 is set, else DGROUP:0x337e; with bit 1 the sprite itself.
 *
 * The two coordinates are `(x, y)` and not `(y, x)`: the fourth argument of `Sprite_DrawMono` is
 * the one the shadow shifts, and `draw_marker` computes it from the window's LEFT
 * edge. `Sprite_Draw`'s own reconstruction takes `px` fourth too.
 *
 * DGROUP:0x337e is a COLOUR, not a glyph -- `CDControlWin_DrawTrack` fills with it and
 * `draw_marker` hands it to the same parameter of `Sprite_DrawMono` beside a literal
 * 0x5f. The name here was `g_cursor_glyph`.
 *
 * RENAMED 2026-09-26 from draw_glyph_or_sprite_kind: the Mac build's PieceIconDraw(GRPort&, short, short, short, short), bound by call-sequence alignment (3.0 aligned gaps in ColonyWin_DrawColonyMap, ColonyWin_DrawFort, ColonyWin_DrawIndividual leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 2.40x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_DrawMono(RECT *buf, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void PieceIconDraw(GRPort *dst, int icon, int x, int y, int flags)
{
    char buf1[8];
    char buf2[8];

    if (flags & 1)
        Sprite_DrawMono((RECT *)buf1, &g_icon_sprites[icon], dst, x - 2, y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw((RECT *)buf2, &g_icon_sprites[icon], dst, x, y);
}
