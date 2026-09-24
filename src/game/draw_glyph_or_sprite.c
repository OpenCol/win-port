/* draw_glyph_or_sprite -- shipped at 1040:2058.
 *
 * Ported by tools/port.py from matched/game/draw_glyph_or_sprite.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw9(void *buf, void *rec, void *dst, col_i16 x, col_i16 y, char c);
extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_glyph_or_sprite(void *dst, void *rec, col_i16 x, col_i16 y, col_i16 flags)
{
    char buf1[8];
    char buf2[8];

    if (flags & 1)
        sprite_draw9(buf1, rec, dst, x - 2, y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        sprite_draw(buf2, rec, dst, x, y);
}
