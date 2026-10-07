/* delete_village: candidate for 1030:82f0 (410 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: delete_village
 * name-source: sibling-build
 *
 * Removes native settlement s: its plane-1 bit 1 is cleared, native units belonging to it
 * are removed (later settlement indices shift down by one), the table closes up, the tribe's
 * settlement count (bytes at SEG20:0xdc10 by current tribe) drops; a tribe with settlements
 * left has its record's byte +8 and word +0xa reduced by a (count + 1)th, a tribe with none
 * is marked destroyed (+3 bit 7) and EXTINCT announced with its name.
 *
 * RENAMED 2026-09-26 from native_settlement_delete: the Mac build's delete_village(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller combat_fight; caller orders_disband; callee delete_unit; callee popm; callee popup_say), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "terrain.h"
#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_village(int s);
extern void feature_set(int x, int y, int mask, int on);
extern void delete_unit(int u);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void popup_say(int slot, int word);
extern "C" int popm(char *msg, int n);   /* PORT: C linkage, as defined (W2) */

void delete_village(int s)
{
    int i;

    set_village(s);
    feature_set(g_settlements[s].x, g_settlements[s].y, MAP1_SETTLEMENT, 0);
    for (i = g_game.unit_count - 1; i >= 0; i--) {
        if ((unsigned char)(g_units[i].flags & UNIT_NATION_MASK) >= 4) {
            if (g_units[i].home == (signed char)s)
                delete_unit(i);
            else if (g_units[i].home > (signed char)s)
                g_units[i].home--;
        }
    }
    for (i = s; g_game.settlement_count - 1 > i; i++)
        g_settlements[i] = g_settlements[i + 1];
    g_game.settlement_count--;
    g_tribe_settlement_count[g_cur_tribe]--;
    if (g_tribe_settlement_count[g_cur_tribe] > 0) {
        g_cur_tribe_rec->horse_herds -= g_cur_tribe_rec->horse_herds / (g_tribe_settlement_count[g_cur_tribe] + 1);
        g_cur_tribe_rec->horses -= g_cur_tribe_rec->horses / (g_tribe_settlement_count[g_cur_tribe] + 1);
    } else {
        g_cur_tribe_rec->flags |= 0x80;
        popup_say(0, get_adjective(g_cur_tribe_nation));
        popm("EXTINCT", 3);
    }
}
