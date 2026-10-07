/* delete_unit: candidate for 1038:2532 (324 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Removes unit u: the owner's unit count (bytes at DGROUP:0x4b44) drops for a European
 * owner, a native unit's settlement (+6) gets bit 0 of its +3 byte set, the record's
 * bookkeeping runs (pick_up_unit), the array closes up by one record, every link
 * (+0x18, +0x1a) above u is decremented, and so is the selected unit (SEG20:0x7794) when it
 * was at or past u.
 *
 * RENAMED 2026-09-26 from unit_record_free: the Mac build's delete_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller DoUnitArticle; caller foreign_country; caller indian_move; caller plan_move; caller route_edit), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "nations.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void pick_up_unit(int u);
void delete_unit(int u)
{
    int owner;
    int i;

    if (u < 0)
        goto done;
    owner = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
    if (owner >= 0 && owner < NATION_EUROPEAN_COUNT)
        g_nation_unit_count[owner]--;
    if (owner >= NATION_EUROPEAN_COUNT) {
        if ((int)g_units[u].home >= 0)
            g_settlements[g_units[u].home].flags |= 1;
    }
    pick_up_unit(u);
    for (i = u; g_game.unit_count - 1 > i; i++)
        g_units[i] = g_units[i + 1];
    g_game.unit_count--;
    for (i = 0; g_game.unit_count > i; i++) {
        if (g_units[i].link > u)
            g_units[i].link--;
        if (g_units[i].link2 > u)
            g_units[i].link2--;
    }
    if (g_game.selected_unit != 0 && g_game.selected_unit >= u)
        g_game.selected_unit--;
done:
    ;
}
