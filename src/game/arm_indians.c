/* arm_indians: candidate for 1010:c146 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Arms the pending request at DGROUP:0xa10 and stores its two words at
 * 0x4b9a/0x4b9c.
 *
 * RENAMED 2026-09-26 from set_pending_pair: the Mac build's arm_indians(short, short), bound by call-sequence alignment (2.0 aligned gaps in ColonyWin_ColonyMouseMap, colony_map_click leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.62x this one. It stores its two arguments and arms a pending request that a later step carries out; call-graph votes and uneven-gap layout had pointed here too.
 */

#include "colonization.h"

void arm_indians(int col, int row)
{
    g_pending_flag = 1;
    g_pending_col = col;
    g_pending_row = row;
}
