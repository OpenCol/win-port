/* nth_cargo_vessel: candidate for 1018:7b60 (136 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The n-th unit with a hold on the current colony's tile (the order of
 * colony_cargo_units), or -1.
 *
 * RENAMED 2026-09-26 from colony_nth_cargo_unit: the Mac build's nth_cargo_vessel(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DrawColonyShips; caller KeyClick; caller colony_click_ship; callee top_unit_in_square), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int nth_cargo_vessel(int carrier)
{
    register int u;
    register int i = -1;
    int found = -1;

    for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = next_unit(u)) {
        if (g_unit_types[g_units[u].type].holds != 0) {
            i++;
            if (i == carrier)
                found = u;
        }
    }
    return found;
}
