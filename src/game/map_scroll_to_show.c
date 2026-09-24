/* map_scroll_to_show -- shipped at 1038:b07d.
 *
 * Ported by tools/port.py from matched/game/map_scroll_to_show.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_view_x1;                            DGROUP:0x4c48
 *   extern int far g_view_y1;                            DGROUP:0x4c4a
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern char far g_map_window[];                      SEG20:0x0658
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_e46c(col_i16 a, col_i16 b);
col_i16 min_int_1038_e446(col_i16 a, col_i16 b);
void MapWin_Recenter(void *w, col_i16 x, col_i16 y);
void map_goto(col_i16 x, col_i16 y, col_i16 flag);

col_i16 map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 flag)
{
    register col_i16 r = 0;
    col_i16 xmin;
    col_i16 ymin;
    col_i16 xmax;
    col_i16 ymax;

    xmin = min_int_1038_e446(x1, x2);
    xmax = max_int_1038_e46c(x1, x2);
    ymin = min_int_1038_e446(y1, y2);
    ymax = max_int_1038_e46c(y1, y2);
    if (g_view_x0 + 4 > xmin && g_view_x0 > 1)
        r = 1;
    if (g_view_y0 + 4 > ymin && g_view_y0 > 1)
        r = 1;
    if (g_view_x1 - 4 < xmax && g_view_x1 < (*g_map_width) - 4)
        r = 1;
    if (g_view_y1 - 4 < ymax && g_view_y1 < g_map_h - 4)
        r = 1;
    if (r != 0) {
        map_goto(x1, y1, flag);
        MapWin_Recenter((&g_map_window), x1, y1);
    }
    return r;
}
