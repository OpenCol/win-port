/* set_village: candidate for 1038:004a (104 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Records settlement s as current (index at DGROUP:0x4c34, record pointer at 0x4c2c into
 * the 0x12-byte records at SEG20:0xcb9a; index 0 when out of range) and makes its tribe
 * (byte +2, minus 4) the current tribe. Negative s only stores the index.
 *
 * RENAMED 2026-09-26 from set_current_native_settlement: the Mac build's set_village(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller ReportsIndian; caller combat_analysis; caller combat_fight; caller indian_move; caller move_unit), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_tribe(int tribe);
void set_village(int s)
{
    g_cur_settlement = s;
    if (s < 0)
        goto done;
    if (s < 0 || g_game.settlement_count <= s)
        s = 0;
    g_cur_settlement_rec = &g_settlements[s];
    set_tribe(g_cur_settlement_rec->owner - 4);
done:
    ;
}
