/* draw_glyph_or_sprite_kind -- shipped at 1040:1fcc.
 *
 * Ported by tools/port.py from matched/game/draw_glyph_or_sprite_kind.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Sprite far g_icon_sprites[];                  SEG20:0x6246
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw9(void *buf, void *rec, void *dst, col_i16 x, col_i16 y, char c);
extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_glyph_or_sprite_kind(void *dst, col_i16 kind, col_i16 x, col_i16 y, col_i16 flags)
{
    char buf1[8];
    char buf2[8];

    if (flags & 1)
        sprite_draw9(buf1, &g_icon_sprites[kind], dst, x - 2, y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        sprite_draw(buf2, &g_icon_sprites[kind], dst, x, y);
}
