/* get_colony_defense: candidate for 1020:7e1b (70 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 70, not the census 65. The last 5 bytes are an unreachable epilogue;
 * Ghidra sizes by reachable code.
 *
 * How many of buildings 0, 1 and 2 the current colony has, counted one `if` at a time
 * rather than in a loop -- three unrolled calls to `has_building`, which
 * `try_to_build` already establishes as the colony's building bits.
 *
 * The name is from the two callers, both defensive. One turns the count into a percentage
 * with `plus_string` and `n * 50 + 50`, which is +50/+100/+150/+200%; the other doubles it
 * into a running defence score next to `terrain_at` and the colony's own tile. Three
 * consecutive building bits behaving as one level is a fortification chain.
 *
 * RENAMED 2026-09-26 from colony_fort_level: the Mac build's get_colony_defense(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller combat_analysis; caller get_defense; callee has_building), the runner-up gets 2, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "building.h"
extern int has_building(int bit);

int get_colony_defense(void)
{
    register int n = 0;

    if (has_building(BUILDING_STOCKADE) != 0)
        n++;
    if (has_building(BUILDING_FORT) != 0)
        n++;
    if (has_building(BUILDING_FORTRESS) != 0)
        n++;
    return n;
}
