/* next_unit: candidate for 1038:18ee (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The unit's second link (+0x1a), or u itself when u is negative.
 *
 * RENAMED 2026-09-26 from unit_chain_next: the Mac build's next_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 15 of its Mac call edges already bound land on this function (caller DrawColonyMap; caller DrawColonyUnits; caller DrawInbound; caller DrawOutBound; caller DrawUnits), the runner-up gets 6, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int next_unit(int u)
{
    if (u >= 0)
        u = g_units[u].link2;
    return u;
}
