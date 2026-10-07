/* cargo_to_ship: candidate for 1018:8854 (151 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: cargo_to_ship
 *
 * Moves goods from the current colony's warehouse onto unit u: the amount is
 * the stock capped at 100 and at `max`, left in DGROUP:0x4bd2, taken off the
 * stock and given to stow_cargo; a unit not under orders 2 has its
 * orders cleared.  Returns an uninitialised frame slot (the shipped code does).
 * The two helpers are -called and defined in-file.
 *
 * RENAMED 2026-09-26 from colony_load_goods: the Mac build's cargo_to_ship(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller attempt_cargo_to_ship; callee stow_cargo), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int g_stub_a;

extern int stow_cargo(int u, int goods, int amount);

int cargo_to_ship(int u, int goods, int max)
{
    int result;

    g_cargo_amount = min_int_1018_b329(g_cur_colony_rec->stock[goods], 100);
    g_cargo_amount = min_int_1018_b329(g_cargo_amount, max);
    g_cur_colony_rec->stock[goods] -= g_cargo_amount;
    stow_cargo(u, goods, g_cargo_amount);
    if (g_units[u].orders != ORDERS_TRADE_ROUTE)
        g_units[u].orders = ORDERS_NONE;
    return result;
}
