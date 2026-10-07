/* pissed: candidate for 1038:0139 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Word i of the short array at +0x46 of tribe t's 0x4e-byte record.
 *
 * RENAMED 2026-09-26 from table_78_word: tribe t's anger toward nation i, g_tribes[t].anger[i] -- the short
 * array at +0x46 of the tribe record.
 *
 * RENAMED 2026-09-26 from tribe_anger: the Mac build's pissed(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller ReportsIndian; caller Stats; caller combat_fight; caller compute_size; caller move_unit), the runner-up gets 6, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

int pissed(int tribe, int i)
{
    return g_tribes[tribe].anger[i];
}
