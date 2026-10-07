/* draw_sprite_with_shadow: candidate for 1040:2058 (123 bytes, 5 callers).  Verdict: RELOCATABLE.
 *
 * With bit 0 of flags, draws the same sprite record two pixels left through
 * Sprite_DrawMono as a solid-colour shadow -- colour 0x5f when bit 2 is set, else
 * g_draw_color; with bit 1, the sprite at (x, y) through
 * Sprite_Draw. Two separate 8-byte scratch buffers.
 *
 * The two coordinates are `(x, y)` and not `(y, x)`: the fourth argument of `Sprite_DrawMono` is
 * the one the shadow shifts, and `draw_marker` computes it from the window's LEFT
 * edge. `Sprite_Draw`'s own reconstruction takes `px` fourth too.
 *
 * DGROUP:0x337e is a COLOUR, not a glyph -- `CDControlWin_DrawTrack` fills with it and
 * `draw_marker` hands it to the same parameter of `Sprite_DrawMono` beside a literal
 * 0x5f. The name here was `g_cursor_glyph`.
 *
 * RENAMED 2026-09-26 from draw_glyph_or_sprite: Bit 0 draws the SAME sprite record through Sprite_DrawMono, whose last argument is a blit colour (0x5f, or g_draw_color at DGROUP:0x337e, which the header itself says is a colour and not a glyph), 2 pixels left of (x, y). That is a solid-colour silhouette. Bit 1 then draws the sprite normally at (x, y). draw_marker does the same thing with +2/-2 offsets. No glyph or character is drawn, and the two are not alternatives ('or'): PieceDraw calls it with 1 and then 2 to get the shadow and then the icon on top of it.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"
extern RECT *Sprite_DrawMono(RECT *buf, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void draw_sprite_with_shadow(void *dst, void *rec, int x, int y, int flags)
{
    char buf1[8];
    char buf2[8];

    if (flags & 1)
        Sprite_DrawMono((RECT *)buf1, (Sprite *)rec, (GRPort *)dst, x - 2, y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw((RECT *)buf2, (Sprite *)rec, (GRPort *)dst, x, y);
}
