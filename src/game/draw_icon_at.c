/* draw_icon_at -- shipped at 1038:d8a0.
 *
 * Ported by tools/port.py from matched/game/draw_icon_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern SpriteRec far g_sprites_4506[];               SEG20:0x4506
 *
 * The file's own SpriteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_icon_at(void *dst, unsigned char kind, col_i16 x, col_i16 y)
{
    char buf[8];

    sprite_draw(buf, &g_sprites_4506[kind], dst, x, y);
}
