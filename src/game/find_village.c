/* find_village: candidate for 1038:0548 (112 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The index of the native village at (x, y) among the 18-byte records at
 * SEG20:0xcb9a (count at SEG20:0x779c), or -1 when the tile has no native
 * owner or no village.
 *
 * RENAMED 2026-09-26 from village_at: the Mac build's find_village(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller Stats; caller combat_fight; caller move_unit; caller orders_disband; caller tribe_village), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int is_village(int x, int y);

int find_village(int x, int y)
{
    register int found = -1;
    register int i;

    if (is_village(x, y) < 0)
        goto done;
    for (i = 0; found < 0 && i < g_game.settlement_count; i++) {
        if (g_settlements[i].x == (unsigned char)x && g_settlements[i].y == (unsigned char)y)
            found = i;
    }
done:
    return found;
}
