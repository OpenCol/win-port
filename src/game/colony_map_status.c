/* colony_map_status: candidate for 1018:3f16 (47 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_map_status
 *
 * 2 + min(2, count of building 10's levels): how the colony's land
 * reaches.  Both helpers are -called and defined in-file.
 *
 * RENAMED 2026-09-26 from colony_ring_level: the Mac build's colony_map_status(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_field_status; caller colony_plan; caller eligible_to_build; caller indian_turns; callee building_level), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "building.h"
extern int g_stub_a;

extern int building_level(int bit);

int colony_map_status(void)
{
    register int n;

    n = building_level(BUILDING_TOWN_HALL_2);
    n = min_int_1018_b329(2, n);
    return n + 2;
}
