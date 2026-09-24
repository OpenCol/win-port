/* redraw_map_view -- shipped at 1040:12d5.
 *
 * Ported by tools/port.py from matched/game/redraw_map_view.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_view_w;                             DGROUP:0x4c50
 *   extern int far g_view_h;                             DGROUP:0x4c52
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void map_view_layout_1040_0597(void);

extern void map_draw_region(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 a);



void redraw_map_view(col_i16 a)
{
    map_view_layout_1040_0597();
    map_draw_region(g_view_x0, g_view_y0, g_view_w, g_view_h, a);
}
