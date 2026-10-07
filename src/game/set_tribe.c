/* set_tribe: candidate for 1038:0000 (74 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Records tribe t as current: the raw index at DGROUP:0x4c30, the index plus 4 (its
 * nation number) at 0x4c32, and a pointer to its 0x4e-byte record at 0x4c28. An index
 * outside 0..7 selects record 0 for the latter two.
 *
 * RENAMED 2026-09-26 from set_current_tribe: the Mac build's set_tribe(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller ReportsIndian; caller Stats; caller combat_fight; caller indian_move; caller move_unit), the runner-up gets 6, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

void set_tribe(int tribe)
{
    g_cur_tribe = tribe;
    if (tribe < 0 || tribe >= 8)
        tribe = 0;
    g_cur_tribe_nation = tribe + 4;
    g_cur_tribe_rec = &g_tribes[tribe];
}
