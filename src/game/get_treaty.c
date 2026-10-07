/* get_treaty: candidate for 1030:7e2f (87 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Byte b of nation a's pair-flag array: nations 0..3 use the 0x13c-byte nation records,
 * 4 and up the 0x4e-byte tribe records (0xfe bytes before each record's start).
 *
 * RENAMED 2026-09-26 from nation_pair_flags_get: the Mac build's get_treaty(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller ReportsForeign; caller ReportsIndian; caller combat_fight; caller foreign_turn; caller move_unit), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

int get_treaty(int nation_a, int nation_b)
{
    int treaty;

    if (nation_a >= 4)
        treaty = ((unsigned char *)&g_tribes[nation_a])[nation_b - 0xfe];
    else
        treaty = g_nations[nation_a].rel[nation_b];
    return treaty;
}
