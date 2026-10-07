/* issue_stack_orders: candidate for 1038:35bd (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: issue_stack_orders
 *
 * Sets the orders byte (Unit+8) of every unit from the tail of u's chain.
 * Both helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_chain_set_orders: the Mac build's issue_stack_orders(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between any_ship_in_stack and stack_units, which are already bound; the arity agrees and the Mac body is 1.43x this one.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

void issue_stack_orders(int u, int orders)
{
    for (u = top_unit(u); u >= 0; u = next_unit(u))
        g_units[u].orders = orders;
}
