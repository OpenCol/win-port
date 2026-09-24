/* draw_colonies -- shipped at 1038:de64.
 *
 * Ported by tools/port.py from matched/game/draw_colonies.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int g_view_x0;                                DGROUP:0x4c44
 *   extern int g_view_y0;                                DGROUP:0x4c46
 *   extern int g_view_x1;                                DGROUP:0x4c48
 *   extern int g_view_y1;                                DGROUP:0x4c4a
 *   extern int g_tile_w;                                 DGROUP:0x4c4c
 *   extern int g_tile_h;                                 DGROUP:0x4c4e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* SEG20:0x7c28 */








extern void draw_colony(void *w, col_i16 i, col_i16 px, col_i16 py, col_i16 size, col_i16 a, col_i16 b);

void draw_colonies(void *w)
{
    unsigned char i;
    unsigned char x;
    unsigned char y;

    for (i = 0; i < g_colony_count; i++) {
        x = g_colonies[i].x;
        y = g_colonies[i].y;
        if (x >= g_view_x0 && x <= g_view_x1 && y >= g_view_y0 && y <= g_view_y1)
            draw_colony(w, i, (x - g_view_x0) * g_tile_w, (y - g_view_y0) * g_tile_h,
                        0x10, 0, 0);
    }
}
