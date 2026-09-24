/* unit_arrive_nearest -- shipped at 1030:310d.
 *
 * Ported by tools/port.py from matched/game/unit_arrive_nearest.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern unsigned char far g_view_nation;              SEG20:0x7798
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1030_ead6(col_i16 a, col_i16 b);
col_i16 square_accepts_unit(col_i16 u, col_i16 x, col_i16 y);

#include "units.h"
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern void unit_chain_free_all(col_i16 u);
extern void unit_chain_apply_270a(col_i16 u);
extern void unit_chain_relink(col_i16 u, col_i16 x, col_i16 y);
extern void unit_join_nation_units(col_i16 u);
extern void explore_square(col_i16 u);
extern col_i16 map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 m);
extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 a);
extern void viceroy_unit_msg6(col_i16 u);

/* same object: 4-byte `push cs; call ` */



void unit_arrive_nearest(col_i16 u)
{
    col_i16 x0;
    col_i16 y0;
    col_i16 j;
    col_i16 i;
    col_i16 fx;
    col_i16 fy;
    col_i16 found;
    col_i16 r;
    col_i16 other;
    register col_i16 x;
    register col_i16 y;

    found = 0;
    r = 0;
    fx = x0 = g_units[u].dest_x;
    fy = y0 = g_units[u].dest_y;
    do {
        for (x = x0 - r; found == 0 && x0 + r >= x; x++) {
            for (i = -1; i <= 1; i += 2) {
                y = (i != 0 ? (i >= 0 ? r : ~r + 1) : 0) + y0;
                if (square_accepts_unit(u, x, y) != 0) {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
        for (y = y0 - r; found == 0 && y0 + r >= y; y++) {
            for (j = -1; j <= 1; j += 2) {
                x = (j != 0 ? (j >= 0 ? r : ~r + 1) : 0) + x0;
                if (square_accepts_unit(u, x, y) != 0) {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
        r++;
    } while (found == 0 && max_int_1030_ead6((*g_map_width), g_map_h) > r);
    if (found == 0) {
        other = unit_at(fx, fy);
        unit_chain_free_all(other);
    }
    unit_chain_apply_270a(u);
    unit_chain_relink(u, fx, fy);
    unit_join_nation_units(u);
    explore_square(u);
    if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == g_view_nation) {
        if (map_scroll_to_show(fx, fy, fx, fy, 0) == 0)
            redraw_map_area(fx - 3, fy - 3, 7, 7, 1);
        viceroy_unit_msg6(u);
    }
}
