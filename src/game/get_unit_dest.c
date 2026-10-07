/* get_unit_dest: candidate for 1038:307a (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The high nibble of the unit's route byte (Unit+0x17), arithmetic-shifted,
 * so the field is signed.
 *
 * RENAMED 2026-09-26 from unit_route_flag_get: the Mac build's get_unit_dest(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller kill_dest; caller orders_route; caller perform_orders_route), the runner-up gets 1, the arity agrees (1), and the Mac body is 1.26x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

#include "records.h"

int get_unit_dest(int u)
{
    return g_units[u].spec >> 4;
}
