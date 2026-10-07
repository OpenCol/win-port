/* orders_route: candidate for 1048:a3bd (255 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Trade Route command for the selected unit (SEG20:0x7794): TRADENONE when no
 * routes exist; otherwise the route picker (kind 2 for ships, types 13..18, else 1) and, on
 * a choice, the unit's route and stop are set, `dest_get` (titled SAILPORT for a sea route, TRAVELPLACE otherwise) picks the stop to head for, pre-selecting the current one, the
 * unit's state (+8) becomes 2 and the single-port check runs.
 *
 * RENAMED 2026-09-26 from begin_trade_route: the Mac build's orders_route(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller MainMenuProc; callee perform_orders_route; callee popm; callee route_get), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int popm(char *msg, int n);
extern int get_unit_route(int u);
extern void set_unit_route(int u, int r);
extern int get_unit_dest(int u);
extern void set_unit_dest(int u, int f);
extern int route_get(int kind, int route, char *msg);
extern void set_dest(int r);
extern int dest_get(int cur);
extern void perform_orders_route(int u, int flag);

void orders_route(void)
{
    int u;
    int r;
    int cur;

    if (g_game.trade_route_count == 0) {
        popm("TRADENONE", 0);
        goto done;
    }
    u = g_game.selected_unit;
    r = route_get((g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) ? 2 : 1,
                                get_unit_route(u), "TRADESELECT");
    if (r < 0)
        goto done;
    if (get_unit_route(u) != r) {
        set_unit_route(u, r);
        set_unit_dest(u, 0);
    }
    cur = get_unit_dest(u);
    set_dest(r);
    cur = dest_get(cur);
    if (cur < 0)
        goto done;
    set_unit_dest(u, cur);
    g_units[u].orders = ORDERS_TRADE_ROUTE;
    perform_orders_route(u, 1);
done:
    ;
}
