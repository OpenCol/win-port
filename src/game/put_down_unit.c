/* put_down_unit: candidate for 1038:1ca8 (234 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: put_down_unit
 *
 * Puts unit u on tile (x, y): position stored, link cleared, link2 set to
 * the tile's first unit, which in turn links to u; on an empty map tile
 * plane-1 MAP1_UNIT (0x01) is set and the owner nibble takes the unit's nation unless
 * a native (>= 4) is standing on a tile lost_city_at marks (a Lost City Rumour).  top_unit_in_square is
 * -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from unit_place_at: the Mac build's put_down_unit(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller relocate_unit; caller special_kill; callee feature_set; callee lost_city_at; callee on_map), the runner-up gets 4, and the arity agrees (3).
 */

#include "colonization.h"

#include "terrain.h"
#include "units.h"
#include "records.h"

extern int g_stub_a;

extern void feature_set(int x, int y, int mask, int on);
extern int lost_city_at(int x, int y);
extern void owner_set(int x, int y, int v);

extern int top_unit_in_square(int x, int y);

void put_down_unit(int u, int x, int y)
{
    int unit;

    unit = top_unit_in_square(x, y);
    g_units[u].x = x;
    g_units[u].y = y;
    g_units[u].link = -1;
    g_units[u].link2 = unit;
    if (unit >= 0) {
        g_units[unit].link = u;
    } else if (on_map(x, y) != 0) {
        feature_set(x, y, MAP1_UNIT, 1);
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) < 4 || lost_city_at(x, y) == 0)
            owner_set(x, y, (unsigned char)(g_units[u].flags & UNIT_NATION_MASK));
    }
}
