/* bottom_unit: candidate for 1038:18b0 (62 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * top_unit on the second link field (+0x1a).
 *
 * RENAMED 2026-09-26 from unit_chain26_tail: the Mac build's bottom_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller combat_fight; caller move_to_bottom; caller plan_move), the runner-up gets 2, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

int bottom_unit(int u)
{
    if (u >= 0)
        while (g_units[u].link2 >= 0)
            u = g_units[u].link2;
    return u;
}
