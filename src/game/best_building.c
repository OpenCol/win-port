/* best_building: candidate for 1018:3ece (72 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the building chain from b while the current colony has each flag; returns the
 * last one it had, or -1.
 *
 * RENAMED 2026-09-26 from colony_building_top: the Mac build's best_building(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller compute_colonist_yield; callee has_building), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int has_building(int bit);
int best_building(int bit)
{
    int top = -1;

    while (bit >= 0) {
        if (!has_building(bit))
            break;
        top = bit;
        bit = g_buildings[bit].next_level;
    }
    return top;
}
