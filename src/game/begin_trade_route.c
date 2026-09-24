/* begin_trade_route -- shipped at 1048:a3bd.
 *
 * Ported by tools/port.py from matched/game/begin_trade_route.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_trade_route_count;                  SEG20:0x77a2
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */



extern void text_message_show(char *msg, col_i16 n);
extern col_i16 unit_route_get(col_i16 u);
extern void unit_route_set(col_i16 u, col_i16 r);
extern col_i16 unit_route_flag_get(col_i16 u);
extern void unit_route_flag_set(col_i16 u, col_i16 f);
extern col_i16 no_trade_routes_defined(col_i16 kind, col_i16 route, char *msg);
extern void set_current_route(col_i16 r);
extern col_i16 route_pick_stop(col_i16 f);
extern void trade_route_single_port_check(col_i16 u, col_i16 flag);

void begin_trade_route(void)
{
    col_i16 u;
    col_i16 r;
    col_i16 f;

    if (g_trade_route_count == 0) {
        text_message_show("TRADENONE", 0);
        goto done;
    }
    u = g_selected_unit;
    r = no_trade_routes_defined((g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) ? 2 : 1,
                                unit_route_get(u), "TRADESELECT");
    if (r < 0)
        goto done;
    if (unit_route_get(u) != r) {
        unit_route_set(u, r);
        unit_route_flag_set(u, 0);
    }
    f = unit_route_flag_get(u);
    set_current_route(r);
    f = route_pick_stop(f);
    if (f < 0)
        goto done;
    unit_route_flag_set(u, f);
    g_units[u].orders = ORDERS_TRADE_ROUTE;
    trade_route_single_port_check(u, 1);
done:
    ;
}
