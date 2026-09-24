/* redraw_arrived_area -- shipped at 1040:1203.
 *
 * Ported by tools/port.py from matched/game/redraw_arrived_area.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void clip_xywh_to_map_rect(void *x, void *y, void *w, void *h);
void map_blit_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
void map_copy_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
void refresh_arrived_unit_1040_0e2e(col_i16 flag);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */









void redraw_arrived_area(col_i16 flag)
{
    col_i16 x;
    col_i16 y;
    col_i16 w;
    col_i16 h;

    x = g_units[g_selected_unit].x;
    y = g_units[g_selected_unit].y;
    w = h = 1;
    clip_xywh_to_map_rect(&x, &y, &w, &h);
    map_copy_tiles(x, y, w, h);
    refresh_arrived_unit_1040_0e2e(flag);
    map_blit_tiles(x, y, w, h);
}
