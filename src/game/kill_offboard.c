/* kill_offboard: candidate for 1038:48fe (211 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: kill_offboard
 *
 * Frees every unit of `nation` that is off the map (at sea, in Europe): a
 * ship among them (type 13..18) is announced with its type name in text
 * slot 0 and the SEIZURE text.  delete_unit is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from seize_ship_on_high_seas: the Mac build's kill_offboard(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee delete_unit; callee on_map; callee popm; callee popup_say), the runner-up gets 3, the arity agrees (1), and the Mac body is 1.14x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says. The old name described one message; the body frees EVERY unit of the nation that is off the map, ships announced -- kill_offboard.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);

extern void delete_unit(int u);

void kill_offboard(int nation)
{
    register int u;

    for (u = g_game.unit_count - 1; u >= 0; u--) {
        if ((char)(g_units[u].flags & UNIT_NATION_MASK) != (char)nation)
            continue;
        if (on_map(g_units[u].x, g_units[u].y) == 0) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                popup_say(0, g_unit_types[g_units[u].type].name);
                popm("SEIZURE", 0);
            }
            delete_unit(u);
        }
    }
}
