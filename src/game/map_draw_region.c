/* map_draw_region -- shipped at 1040:19da.
 *
 * Ported by tools/port.py from matched/game/map_draw_region.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window[];                      SEG20:0x0658
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_view_x1;                            DGROUP:0x4c48
 *   extern int far g_view_y1;                            DGROUP:0x4c4a
 *   extern int far g_tile_w;                             DGROUP:0x4c4c
 *   extern int far g_tile_h;                             DGROUP:0x4c4e
 *   extern int far g_map_pixels;                         DGROUP:0x4b2e
 *   extern unsigned char huge *g_draw_p1;                DGROUP:0x4c7e
 *   extern unsigned char huge *g_map_tiles2;             DGROUP:0x4c82
 *   extern unsigned char huge *g_draw_p3;                DGROUP:0x4c86
 *   extern unsigned char huge *g_draw_pix;               DGROUP:0x4c8a
 *   extern int g_draw_x;                                 DGROUP:0x4c8e
 *   extern int g_draw_y;                                 DGROUP:0x4c90
 *   extern unsigned char g_draw_mask;                    DGROUP:0x4c92
 *   extern int g_draw_px;                                DGROUP:0x4c94
 *   extern int g_draw_py;                                DGROUP:0x4c96
 *   extern int g_draw_parity;                            DGROUP:0x4c98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1040_445d(col_i16 a, col_i16 b);
col_i16 min_int_1040_4483(col_i16 a, col_i16 b);
void map_draw_square(col_i16 edge);
void map_view_layout_1040_0597(void);

extern col_i16 MapWin_GetZoom(char *w);
extern void scale_map_select(col_i16 num, col_i16 den);
extern unsigned char * map_plane0_ptr(col_i16 x, col_i16 y);
extern unsigned char * map_plane1_ptr(col_i16 x, col_i16 y);
extern unsigned char * map_plane3_ptr(col_i16 x, col_i16 y);
extern unsigned char * MSHLock(col_i16 h);
extern unsigned char * MSHUnlock(col_i16 h);
/* same object: 4-byte `push cs; call ` (rule 47) */





void map_draw_region(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 layer)
{
    col_i16 row;
    col_i16 x2;
    col_i16 y2;
    col_i16 px;
    col_i16 py;
    col_i16 width;
    col_i16 inner_row;
    col_i16 edge;
    unsigned char *p0;
    unsigned char *p1;
    unsigned char *p3;
    unsigned char *pix;
    col_i16 num;
    col_i16 den;
    register col_i16 col;
    register col_i16 inner_col;

    num = MapWin_GetZoom((&g_map_window)) + 8;
    den = 8;
    scale_map_select(num, den);
    if (layer >= 0)
        g_draw_mask = (unsigned char)(1 << layer) << 4;
    else
        g_draw_mask = 0;
    map_view_layout_1040_0597();
    if (g_view_x1 < x)
        goto out;
    if (g_view_y1 < y)
        goto out;
    x2 = x + w - 1;
    y2 = y + h - 1;
    x = max_int_1040_445d(x, g_view_x0);
    y = max_int_1040_445d(y, g_view_y0);
    x2 = min_int_1040_4483(x2, g_view_x1);
    y2 = min_int_1040_4483(y2, g_view_y1);
    w = x2 - x + 1;
    h = y2 - y + 1;
    col = max_int_1040_445d(1, x);
    row = max_int_1040_445d(1, y);
    px = (col - g_view_x0) * g_tile_w;
    py = (row - g_view_y0) * g_tile_h;
    p0 = map_plane0_ptr(col, row);
    p1 = map_plane1_ptr(col, row);
    p3 = map_plane3_ptr(col, row);
    width = (*g_map_width);
    pix = MSHLock(g_map_pixels);
    pix += (col << 2) + (((*g_map_width) * row) << 2);
    g_draw_py = py;
    for (row = y; row <= y2; row++) {
        g_draw_y = row;
        g_map_tiles2 = p0;
        g_draw_p1 = p1;
        g_draw_p3 = p3;
        g_draw_pix = pix;
        inner_row = row > 0 && g_map_h - 1 > row;
        g_draw_parity = 0;
        g_draw_px = px;
        for (col = x; col <= x2; col++) {
            g_draw_x = col;
            inner_col = col > 0 && (*g_map_width) - 1 > col;
            edge = inner_col == 0 || inner_row == 0;
            map_draw_square(edge);
            g_draw_px += g_tile_w;
            if (inner_col != 0 && inner_row != 0) {
                g_map_tiles2++;
                g_draw_p1++;
                g_draw_p3++;
                g_draw_pix += 4;
            }
            g_draw_parity ^= 1;
        }
        g_draw_py += g_tile_h;
        if (inner_row != 0) {
            p0 += width;
            p1 += width;
            p3 += width;
            pix += width << 2;
        }
    }
out:
    g_draw_pix = MSHUnlock(g_map_pixels);
    scale_map_select(1, 1);
}
