/* draw_unit_at -- shipped at 1038:d7cb.
 *
 * Ported by tools/port.py from matched/game/draw_unit_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern SpriteRec far g_art_base[];                   SEG20:0x41a6
 *   extern SpriteRec far g_overlay_sprites[];            SEG20:0x5cc6
 *   extern short far g_map_pixels;                       DGROUP:0x4b2e
 *   extern signed char far *g_overlay_plane;             DGROUP:0x4c64
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * The file's own SpriteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_unit_at(void *dst, unsigned char kind, col_i16 x, col_i16 y, col_i16 col, col_i16 row)
{
    signed char *p;
    char buf1[8];
    char buf2[8];
    col_i16 i;

    g_overlay_plane = (signed char *)MSHLock(g_map_pixels);
    sprite_draw(buf1, &g_art_base[kind], dst, x, y);
    p = g_overlay_plane + col * 4;
    p += (*g_map_width) * 4 * row;
    for (i = 0; i < 4; i++, p++) {
        if ((col_i16)*p >= 0)
            sprite_draw(buf2, &g_overlay_sprites[*p], dst, x, y);
    }
    g_overlay_plane = (signed char *)MSHUnlock(g_map_pixels);
}
