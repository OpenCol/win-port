/* possibly_spot_stack: candidate for 1038:2bc0 (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: possibly_spot_stack
 *
 * For each of the four nations whose in_view test passes at the
 * unit's tile, sets that nation on the unit's chain.  Both helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_join_nation_units: the Mac build's possibly_spot_stack(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller move_unit; callee in_view; callee spot_stack), the runner-up gets 2, the arity agrees (1), and the Mac body is 1.32x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int in_view(int x, int y, int nation);
extern void spot_stack(int u, int nation);

void possibly_spot_stack(int u)
{
    register int i;

    for (i = 0; i < 4; i++) {
        if (in_view(g_units[u].x, g_units[u].y, i) != 0)
            spot_stack(u, i);
    }
}
