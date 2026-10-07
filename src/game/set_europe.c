/* set_europe: candidate for 1048:4047 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Selects nation n: the index at DGROUP:0x4cc8 and the pointer to its
 * 316-byte record (SEG20:0xc2f4) at 0x4cc4.
 *
 * RENAMED 2026-09-26 from set_current_nation: the Mac build's set_europe(short), bound by call-graph agreement (tools/mac_callgraph.py): 23 of its Mac call edges already bound land on this function (caller DoHuman; caller DrawEuropeShips; caller DrawEurope; caller DrawInbound; caller DrawLandUnit), the runner-up gets 11, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

void set_europe(int n)
{
    g_cur_nation = n;
    g_cur_nation_rec = &g_nations[g_cur_nation];
}
