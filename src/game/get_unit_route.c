/* get_unit_route: candidate for 1038:301b (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The low nibble of the unit's route byte (Unit+0x17).  The `CBW` before the
 * mask is the signed char promoting.
 *
 * RENAMED 2026-09-26 from unit_route_get: the Mac build's get_unit_route(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller Stats; caller orders_route; caller perform_orders_route; caller route_delete), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

int get_unit_route(int u)
{
    return g_units[u].spec & 0xf;
}
