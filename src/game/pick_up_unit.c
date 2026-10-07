/* pick_up_unit: candidate for 1038:1b8d (283 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Takes unit u out of its chain: the neighbours on either side are joined
 * over it; a unit that had no neighbours clears plane-1 bit 1 of its map
 * tile; its position becomes (-1, -1).
 *
 * RENAMED 2026-09-26 from unit_unlink: the Mac build's pick_up_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller delete_unit; caller move_to_bottom; caller relocate_unit; caller special_kill; callee feature_set), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "terrain.h"
#include "records.h"

extern void feature_set(int x, int y, int mask, int on);

void pick_up_unit(int u)
{
    int linked = 0;

    if (g_units[u].link >= 0) {
        g_units[g_units[u].link].link2 = g_units[u].link2;
        linked = 1;
    }
    if (g_units[u].link2 >= 0) {
        g_units[g_units[u].link2].link = g_units[u].link;
        linked = 1;
    }
    if (linked == 0 && on_map(g_units[u].x, g_units[u].y) != 0)
        feature_set(g_units[u].x, g_units[u].y, MAP1_UNIT, 0);
    g_units[u].x = -1;
    g_units[u].y = -1;
}
