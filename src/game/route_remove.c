/* route_remove -- shipped at 1028:c70e.
 *
 * Ported by tools/port.py from matched/game/route_remove.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern RouteRec far g_routes[];                      SEG20:0xc804
 *   extern int far g_trade_route_count;                  SEG20:0x77a2
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_turn_nation;              SEG20:0x7796
 *
 * The file's own RouteRec, Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
extern col_i16 unit_route_get(col_i16 u);
extern void unit_route_set(col_i16 u, col_i16 r);
extern void unit_route_flag_set(col_i16 u, col_i16 f);

void route_remove(col_i16 idx)
{
    col_i16 r;
    register col_i16 u;
    register col_i16 i;

    for (u = 0; u < g_unit_count; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == g_turn_nation) {
            if (g_unit_types[g_units[u].type].holds > 0) {
                r = unit_route_get(u);
                if (idx == r) {
                    unit_route_set(u, 0);
                    unit_route_flag_set(u, 0);
                    if (g_units[u].orders == ORDERS_TRADE_ROUTE)
                        g_units[u].orders = ORDERS_NONE;
                } else if (r > idx) {
                    unit_route_flag_set(u, r - 1);
                }
            }
        }
    }
    for (i = idx; g_trade_route_count - 1 > i; i++)
        g_routes[i] = g_routes[i + 1];
    g_trade_route_count--;
}
