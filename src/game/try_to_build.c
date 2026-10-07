/* try_to_build: candidate for 1018:0302 (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: try_to_build
 *
 * Tries to take the colony's build slot for a building: 0 when it or a prerequisite in its chain (recursing through each building's prerequisite at +3) is eligible_to_build and becomes the colony's build item, 1 when the chain reaches one the colony already has (or runs out) first. Clears bit 7 of the colony flags when the answer is no.
 *
 * RENAMED 2026-09-26 from building_available: the Mac build's try_to_build(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_plan; callee eligible_to_build; callee has_building), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int has_building(int bit);
extern int eligible_to_build(int item);

int try_to_build(int item)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    int ok = 1;

    if (item < 0)
        goto done;
    if (has_building(item) != 0)
        goto done;
    ok = 0;
    if (eligible_to_build(item) != 0) {
        g_cur_colony_rec->item = item;
        goto done;
    }
    if (try_to_build(g_buildings[item].prev_level) == 0)
        goto done;
    ok = 1;
done:
    if (ok == 0)
        g_cur_colony_rec->status &= 0x7f;
    return ok;
}
