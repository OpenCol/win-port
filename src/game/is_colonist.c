/* is_colonist: candidate for 1018:44e5 (60 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when the unit's type has a colonist class (the signed byte table at
 * SEG21:0x10a is non-negative for it), 0 for ships, wagons and the like.
 *
 * RENAMED 2026-09-26 from unit_is_colonist: the Mac build's is_colonist(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller ColonyClickUnits; caller DrawFort; caller ReportsLabor; caller nth_unit_with_job; caller plan_move), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int is_colonist(int u)
{
    return (int)g_unit_type_class[g_units[u].type] >= 0;
}
