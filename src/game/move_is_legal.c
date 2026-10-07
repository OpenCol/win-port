/* move_is_legal: candidate for 1020:e021 (1087 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1087, not the census 1078: a dead second epilogue and padding before 1020:e460.
 *
 * MAY UNIT `u` STEP ONTO (x, y) WITHOUT ANYTHING BEING SAID? Returns 1 when it may. Otherwise
 * it returns 0 and leaves the reason in DGROUP:0x4c02, which `execute_move_unit` turns into
 * a message or question. The unit is unlinked from its square during the test and relinked after.
 *
 *   code 0  (no message) off the top or bottom row, or off the side for a land unit;
 *   code 4  SAILHOME     a ship moving off the left or right edge;
 *   code 2/3 LANDFALL(2) a ship moving onto land that is not a colony: unowned land, or land
 *                        claimed by the ship's own nation, reached from sea or from its own colony.
 *                        It must carry a unit that can go ashore (type +8 under 99, rated above its
 *                        +5). 3 when the land square has a river. That unit is left at 0x4c04;
 *   code 9  LANDFIRST    a land unit stepping from water onto land another nation holds;
 *   code 8  SHIPLAKE     a ship entering water outside ocean region 1;
 *   code 7  SHIPCOMBAT   a ship other than a privateer, frigate or man-o-war entering water held by another nation;
 *   code 5  SAILHOME     a ship moving east along a sea lane, unless its orders are 2 or 3.
 * A land unit may step onto water only if a ship of its own nation there has room.
 *
 * WHAT THE BYTES SETTLED:
 *   - the terrain is `(unsigned char)(plane0 & 0x1f)`: the mask is applied to the byte, then widened;
 *   - the two nations in the boarding test are compared as bytes;
 *   - the three "not foreign" tests on a ship's landing jump to a label at the end of that branch,
 *     one `goto` each (the shipped JCC-over-JMP form, rule 12).
 *
 * RENAMED 2026-09-26 from move_reason: the Mac build's move_is_legal(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (callee cargo_check; callee continent_at; callee is_city; callee is_colony; callee move_rate), the runner-up gets 4, and the arity agrees (3).
 */

#include "colonization.h"

#include "nations.h"
#include "terrain.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
#define IS_SHIP(t) ((t) >= UNIT_SHIP_FIRST && (t) <= UNIT_SHIP_LAST)

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern int is_city(int x, int y);
extern int is_anything(int x, int y);
extern void stack_units(int u);
extern unsigned char map_get(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int cargo_check(int u, int need, int last_ship);
extern int is_colony(int x, int y);
extern int top_unit(int u);
extern unsigned char move_rate(int u);
extern int next_unit(int u);
extern unsigned char continent_at(int x, int y);
extern int is_unit(int x, int y);
extern void relocate_stacked_units(int u, int x, int y);

int move_is_legal(register int u, register int x, int y)
{
    int ok;
    int p0dst;
    int p0src;
    int tdst;
    int tsrc;
    int type;
    int sx;
    int sy;
    int top;
    int target;
    int owner;
    int nat_dst;
    int any_dst;
    int saved;

    ok = 0;
    g_move_reason = 0;
    if (y < 1)
        goto out;
    if (g_map_size.h - 1 <= y)
        goto out;
    if (g_map_size.w - 1 <= x || x < 1) {
        if (IS_SHIP(g_units[u].type))
            g_move_reason = 4;
        goto out;
    }
    sx = g_units[u].x;
    sy = g_units[u].y;
    nat_dst = is_city(x, y);
    any_dst = is_anything(x, y);
    stack_units(u);
    type = g_units[u].type;
    p0dst = map_get(x, y);
    p0src = map_get(sx, sy);
    tdst = (unsigned char)(p0dst & MAP0_TERRAIN_MASK);
    tsrc = (unsigned char)(p0src & MAP0_TERRAIN_MASK);
    if (tdst == TERRAIN_OCEAN || tdst == TERRAIN_SEA_LANE) {
        if (!IS_SHIP(g_units[u].type)) {
            top = top_unit_in_square(x, y);
            if (top < 0)
                goto relink;
            if ((unsigned char)(g_units[top].flags & UNIT_NATION_MASK) != (unsigned char)(g_units[u].flags & UNIT_NATION_MASK))
                goto relink;
            if (!cargo_check(top, g_unit_types[g_units[u].type].size, 0))
                goto relink;
        }
    }
    if (tdst != TERRAIN_OCEAN && tdst != TERRAIN_SEA_LANE) {
        if (IS_SHIP(g_units[u].type)) {
            if (NATION(g_units[u]) == nat_dst)
                goto land_ok;
            if (nat_dst >= NATION_EUROPEAN_COUNT)
                goto land_ok;
            if (nat_dst >= 0 && nat_dst < NATION_EUROPEAN_COUNT)
                goto land_ok;
            if (any_dst >= 0 && NATION(g_units[u]) != any_dst)
                goto relink;
            if (tsrc == TERRAIN_OCEAN || tsrc == TERRAIN_SEA_LANE || is_colony(sx, sy) >= 0) {
                target = -1;
                saved = u;
                for (u = top_unit(u); target < 0 && u >= 0; u = next_unit(u)) {
                    if (g_unit_types[g_units[u].type].size < 99 && move_rate(u) > g_units[u].moves_used)
                        target = u;
                }
                u = saved;
                if (target >= 0) {
                    g_move_reason_unit = target;
                    if (p0dst & MAP0_RIVER)
                        g_move_reason = 3;
                    else
                        g_move_reason = 2;
                }
            }
            goto relink;
        land_ok:
            ;
        } else {
            if ((tsrc == TERRAIN_OCEAN || tsrc == TERRAIN_SEA_LANE) && any_dst >= 0
                && NATION(g_units[u]) != any_dst) {
                g_move_reason = 9;
                goto relink;
            }
        }
    }
    if (IS_SHIP(g_units[u].type)) {
        if (tdst == TERRAIN_OCEAN || tdst == TERRAIN_SEA_LANE) {
            if (continent_at(x, y) != 1) {
                g_move_reason = 8;
                goto relink;
            }
            owner = is_unit(x, y);
            if (owner >= 0 && NATION(g_units[u]) != owner
                && (g_units[u].type < UNIT_PRIVATEER || g_units[u].type > UNIT_MAN_O_WAR)) {
                g_move_reason = 7;
                goto relink;
            }
        }
        if (tdst == TERRAIN_SEA_LANE && tsrc == TERRAIN_SEA_LANE && sx < x
            && g_units[u].orders != 3 && g_units[u].orders != ORDERS_TRADE_ROUTE) {
            g_move_reason = 5;
            goto relink;
        }
    }
    ok = 1;
relink:
    relocate_stacked_units(u, sx, sy);
out:
    return ok;
}
