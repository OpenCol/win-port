/* get_warehouse: candidate for 1018:46e7 (51 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 100 plus 100 per warehouse level (byte +0x95 of the current colony record).
 *
 * RENAMED 2026-09-26 from colony_warehouse_capacity: the Mac build's get_warehouse(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller DrawMarket; caller colony_plan; caller negotiate_europe; caller new_turn_colony; caller perform_orders_unload), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "records.h"

int get_warehouse(void)
{
    int cap = 100;

    if (g_cur_colony_rec->warehouses != 0)
        cap += g_cur_colony_rec->warehouses * 100;
    return cap;
}
