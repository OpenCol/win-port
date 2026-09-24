/* draw_villages -- shipped at 1038:df27.
 *
 * Ported by tools/port.py from matched/game/draw_villages.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int g_view_x0;                                DGROUP:0x4c44
 *   extern int g_view_y0;                                DGROUP:0x4c46
 *   extern int g_view_x1;                                DGROUP:0x4c48
 *   extern int g_view_y1;                                DGROUP:0x4c4a
 *   extern int g_tile_w;                                 DGROUP:0x4c4c
 *   extern int g_tile_h;                                 DGROUP:0x4c4e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { unsigned char x, y; char rest[16]; } Settlement_local; /* 18 bytes at SEG20:0xcb9a */








extern void draw_native_settlement(void *w, col_i16 v, col_i16 px, col_i16 py, col_i16 mode);

void draw_villages(void *w)
{
    register col_i16 i;
    register col_i16 x;
    col_i16 y;

    for (i = 0; i < g_settlement_count; i++) {
        x = g_settlements[i].x;
        y = g_settlements[i].y;
        if (x >= g_view_x0 && x <= g_view_x1 && y >= g_view_y0 && y <= g_view_y1)
            draw_native_settlement(w, i, (x - g_view_x0) * g_tile_w, (y - g_view_y0) * g_tile_h, 0x10);
    }
}
