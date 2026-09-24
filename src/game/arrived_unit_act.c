/* arrived_unit_act -- shipped at 1008:36b6.
 *
 * Ported by tools/port.py from matched/game/arrived_unit_act.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * The file's own Rec, Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* SEG20:0xa208 */


extern void clear_forest(col_i16 u);
extern void mission_work_step(col_i16 u);
extern void mission_goto_step(col_i16 u);
extern void mission_found_colony(col_i16 u);
extern void trade_route_single_port_check(col_i16 u, col_i16 flag);
extern void unit_set_orders_6(col_i16 u);

void arrived_unit_act(Rec *r)
{
    r->handled = 2;
    switch (g_units[g_selected_unit].orders) {
    case 2: clear_forest(g_selected_unit); break;
    case 3: mission_work_step(g_selected_unit); break;
    case 4: mission_goto_step(g_selected_unit); break;
    case 5: mission_found_colony(g_selected_unit); break;
    case 6: trade_route_single_port_check(g_selected_unit, 0); break;
    case 7: unit_set_orders_6(g_selected_unit); break;
    case 8:
    case 9:
    default: r->handled = 1; break;
    }
}
