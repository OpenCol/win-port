/* unit_arrives -- shipped at 1038:2676.
 *
 * Ported by tools/port.py from matched/game/unit_arrives.c.
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
void unit_detach_attach(col_i16 u, col_i16 a, col_i16 b);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */



extern void explore_square(col_i16 u);
extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 f);
extern col_i16 map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 f);



void unit_arrives(col_i16 u)
{
    col_i16 x;
    col_i16 y;

    if (u >= 0) {
        x = g_units[u].x;
        y = g_units[u].y;
        unit_detach_attach(u, x, y);
        explore_square(u);
        if (map_scroll_to_show(x, y, x, y, 0) == 0)
            redraw_map_area(x - 3, y - 3, 7, 7, 1);
    }
    g_selected_unit = u;
}
