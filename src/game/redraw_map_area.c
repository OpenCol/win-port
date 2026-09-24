/* redraw_map_area -- shipped at 1040:1117.
 *
 * Ported by tools/port.py from matched/game/redraw_map_area.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void clip_xywh_to_map_rect(void *x, void *y, void *w, void *h);
void draw_colonies_view(void);
void draw_settlements_view(void);
void draw_units_in_rect(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
void map_blit_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
void map_copy_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);

extern void map_draw_region(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 n);








void redraw_map_area(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 flag)
{
    clip_xywh_to_map_rect(&x, &y, &w, &h);
    map_draw_region(x, y, w, h, g_reveal_all != 0 ? -1 : g_view_nation);
    draw_settlements_view();
    draw_colonies_view();
    map_copy_tiles(x, y, w, h);
    draw_units_in_rect(x, y, w, h);
    if (flag != 0)
        map_blit_tiles(x, y, w, h);
}
