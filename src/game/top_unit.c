/* top_unit: candidate for 1038:1872 (62 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Follows the unit's link field (+0x18) until it goes negative and returns the last
 * index; a negative start is returned unchanged.
 *
 * RENAMED 2026-09-26 from unit_chain_tail: the Mac build's top_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller Stats; caller attempt_to_set_sail; caller best_defender; caller indian_move; caller move_unit), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int top_unit(int u)
{
    if (u >= 0)
        while (g_units[u].link >= 0)
            u = g_units[u].link;
    return u;
}
