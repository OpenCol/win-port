/* set_unit_route: candidate for 1038:3041 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Low nibble of the unit's route byte (+0x17) := n.
 *
 * RENAMED 2026-09-26 from unit_route_set: the Mac build's set_unit_route(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller orders_route; caller route_delete), the runner-up gets 1, the arity agrees (2), and the Mac body is 1.33x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "records.h"

void set_unit_route(int u, int n)
{
    g_units[u].spec = (g_units[u].spec & 0xf0) | (unsigned char)(n & 0xf);
}
