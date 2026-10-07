/* building_level: candidate for 1018:3e08 (66 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * colony_building_level for the current colony: set flags along the building chain from b.
 *
 * RENAMED 2026-09-26 from current_colony_building_count: the Mac build's building_level(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_plan; caller compute_colonist_yield; caller resolve_raid; callee has_building), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int has_building(int bit);
int building_level(int bit)
{
    int n = 0;

    do {
        if (has_building(bit))
            n++;
        bit = g_buildings[bit].next_level;
    } while (bit >= 0);
    return n;
}
