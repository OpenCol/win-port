/* any_ship_in_stack: candidate for 1038:3553 (106 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: any_ship_in_stack
 *
 * 1 when any unit of u's chain is a ship (type 13..18).  Both chain helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from chain_has_ship: the Mac build's any_ship_in_stack(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller move_unit; caller resolve_raid; callee next_unit; callee top_unit), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

int any_ship_in_stack(int u)
{
    int r = 0;

    for (u = top_unit(u); u >= 0 && r == 0; u = next_unit(u))
        r |= g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST;
    return r;
}
