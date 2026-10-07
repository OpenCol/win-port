/* nth_europe_vessel: candidate for 1048:4a3b (129 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: nth_europe_vessel
 *
 * The nth unit on the current nation's off-map Europe square (g_cur_nation - 0x14 on both
 * axes) whose type has a hold, or -1 when there is no nth one. colony_cargo_units makes the
 * same hold test on a different chain, the current colony's tile.
 *
 * RENAMED 2026-09-26 from nth_cargo_unit: the Mac build's nth_europe_vessel(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DrawEuropeShips; caller EuropeMouseUnits; callee top_unit_in_square), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int nth_europe_vessel(int n)
{
    register int u;
    register int i = -1;
    int found = -1;

    for (u = top_unit_in_square(g_cur_nation - 0x14, g_cur_nation - 0x14); u >= 0;
         u = next_unit(u)) {
        if (g_unit_types[g_units[u].type].holds != 0) {
            i++;
            if (i == n)
                found = u;
        }
    }
    return found;
}
