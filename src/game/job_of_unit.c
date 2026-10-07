/* job_of_unit: candidate for 1018:4521 (49 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The colonist class of the unit's type (signed byte table at SEG21:0x10a,
 * -1 for non-colonists).
 *
 * RENAMED 2026-09-26 from unit_colonist_class: the Mac build's job_of_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DoUnitArticle; caller Score; caller Stats; caller foreign_country; caller get_job), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int job_of_unit(int u)
{
    return g_unit_type_class[g_units[u].type];
}
