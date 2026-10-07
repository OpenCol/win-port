/* set_treaty: candidate for 1030:7e86 (81 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores v as byte b of nation a's pair-flag array (see get_treaty) and returns v.
 *
 * RENAMED 2026-09-26 from nation_pair_flags_set: the Mac build's set_treaty(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller treaty_off; caller treaty_on), the runner-up gets 0, and the arity agrees (3).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

int set_treaty(int nation, int other, int flags)
{
    if (nation >= 4)
        ((unsigned char *)&g_tribes[nation] - 0xfe)[other] = flags;
    else
        g_nations[nation].rel[other] = flags;
    return flags;
}
