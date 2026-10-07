/* spot_stack: candidate for 1038:2795 (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: spot_stack
 *
 * spot_unit(u, n) on every unit of the chain whose tail u leads
 * to.  The three helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_chain_set_nation: the Mac build's spot_stack(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller explore_map_square; caller move_unit; callee next_unit; callee spot_unit; callee top_unit), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);
extern void spot_unit(int u, int n);

void spot_stack(int u, int nation)
{
    u = top_unit(u);
    if (u < 0)
        goto done;
    do {
        spot_unit(u, nation);
        u = next_unit(u);
    } while (u >= 0);
done:
    ;
}
