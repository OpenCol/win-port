/* set_unit_dest: candidate for 1038:30a0 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * High nibble of the unit's route byte (+0x17) := n.
 *
 * RENAMED 2026-09-26 from unit_route_flag_set: the Mac build's set_unit_dest(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller orders_route; caller perform_orders_route; caller route_delete), the runner-up gets 2, and the arity agrees (2). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

void set_unit_dest(int u, int n)
{
    g_units[u].spec = (g_units[u].spec & 0xf) | (unsigned char)(n << 4);
}
