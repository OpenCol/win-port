/* has_father: candidate for 1020:a82e (89 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Bit test in a nation's bit array (0x13c-byte records): 1 for a negative bit index, 0 for
 * a nation index of 4 or more, else the masked byte.
 *
 * RENAMED 2026-09-26 from nation_flag_test: the Mac build's has_father(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller ReportsForeign; caller Score; caller create_colony; caller explore_square), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4; father_bits at +7 */

int has_father(int nation, int bit)
{
    if (bit < 0)
        return 1;
    if (nation >= 4)
        return 0;
    return g_nations[nation].father_bits[bit >> 3] & (1 << (bit & 7));
}
