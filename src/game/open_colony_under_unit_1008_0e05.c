/* open_colony_under_unit_1008_0e05 -- shipped at 1008:0e05.
 *
 * Ported by tools/port.py from matched/game/open_colony_under_unit_1008_0e05.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern char far g_info_win[];                        SEG20:0x1566
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 colony_at(col_i16 x, col_i16 y);
extern void set_current_colony(col_i16 c);
extern col_i16 unload_cheapest_goods(col_i16 u, col_i16 flag);
extern void window_relayout(char *w);

void open_colony_under_unit_1008_0e05(void)
{
    col_i16 x;
    col_i16 y;
    col_i16 sel;
    col_i16 c;

    sel = g_selected_unit;
    x = g_units[sel].x;
    y = g_units[sel].y;
    c = colony_at(x, y);
    if (c < 0)
        goto out;
    set_current_colony(c);
    if (unload_cheapest_goods(sel, 0) != 0)
        goto out;
    window_relayout(g_info_win);
out:
    ;
}
