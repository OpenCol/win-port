/* colony_has_building: candidate for 1018:3da3 (69 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Bit test in the colony's flag bytes (0xca-byte records): 0 for a negative bit index,
 * else the masked byte (non-zero when set).
 *
 * RENAMED 2026-09-26 from colony_flag_get: the Mac build's colony_has_building(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller has_building; caller plan_move; caller special_kill), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

int colony_has_building(int colony, int bit)
{
    if (bit < 0)
        return 0;
    return g_colonies[colony].flags[bit >> 3] & (1 << (bit & 7));
}
