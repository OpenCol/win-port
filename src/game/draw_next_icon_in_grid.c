/* draw_next_icon_in_grid -- shipped at 1058:1d2d.
 *
 * Ported by tools/port.py from matched/game/draw_next_icon_in_grid.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Sprite far g_icon_sprites[];                  SEG20:0x6246
 *   extern int g_layout_x;                               DGROUP:0x4e02
 *   extern int g_layout_y;                               DGROUP:0x4e04
 *   extern Pt far *g_report_rect;                        DGROUP:0x2664
 *
 * The file's own Sprite, Pt dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_next_icon_in_grid(void *dst, col_i16 kind)
{
    char buf[8];

    g_layout_x = g_layout_x + 16;
    if (g_layout_x >= 0x248) {
        g_layout_y += 16;
        g_layout_x = ((g_layout_x >> 1) & 4) + 0x20;
    }
    if (g_layout_y >= 0x120)
        return;
    sprite_draw(buf, &g_icon_sprites[kind], dst, g_report_rect->x + g_layout_x, g_report_rect->y + g_layout_y);
}
