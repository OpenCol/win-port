/* colony_field_status: candidate for 1018:6d0a (963 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 963, not the census 954. The trailing 9 bytes are the dead second epilogue.
 *
 * The status bits of one square of the current colony's 5x5 work area, (cx, cy) counted from its
 * corner, so (2, 2) is the colony itself:
 *
 *   0x10  unusable: off the map, outside the radius `colony_map_status` allows, or
 *         with the colony nation's bit (0x10 << nation) clear in map plane 3 -- nothing else is
 *         worked out
 *   0x80  land owned by another European nation with a unit of that nation on it whose orders are
 *         5 or 6 and which is either of a type whose byte +6 is over 1, or a pioneer (type 2) of
 *         a nation whose byte +0x31 is clear; each such unit is also passed to
 *         `spot_unit` with the colony's nation
 *   0x02  a lost city rumour (`lost_city_at`)
 *   0x04  a native settlement stands on it
 *   0x20  another colony stands on it
 *   0x40  another colony within two squares already works it, per that colony's own slot table
 *   0x08  the centre square
 *
 * Every distance is the spelled-out `d > 0 ? d : ~d + 1` over a subtraction (rules 11 and 19), and
 * the coordinate compares against the settlement and colony tables are byte compares, which is
 * the `(unsigned char)` on the int side.
 *
 * RENAMED 2026-09-26 from colony_tile_flags: the Mac build's colony_field_status(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee next_unit; callee on_map; callee site_get; callee top_unit_in_square), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "nations.h"
#include "units.h"
#define ABS(d) ((d) > 0 ? (d) : ~(d) + 1)

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int on_colony_map(int dx, int dy, int mode);
extern unsigned char site_get(int x, int y);
extern int is_unit(int x, int y);
extern int terrain_is_ocean(int x, int y);
extern int top_unit_in_square(int x, int y);
extern void spot_unit(int u, int n);
extern int next_unit(int u);
extern int lost_city_at(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int colony_map_status(void);
extern int field_index(int x, int y);

int colony_field_status(int cx, int cy)
{
    int x;
    int y;
    int adx;
    int ady;
    int rdx;
    int rdy;
    int owner;
    int ring;
    int u;
    register int i;
    register int flags;

    flags = 0;
    x = g_cur_colony_rec->x + cx - 2;
    y = g_cur_colony_rec->y + cy - 2;
    adx = ABS(cx - 2);
    ady = ABS(cy - 2);
    if (on_map(x, y) == 0 || on_colony_map(adx, ady, colony_map_status()) == 0) {
        flags |= 0x10;
        goto out;
    }
    if (g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
        && (site_get(x, y) & (0x10 << g_cur_colony_rec->nation)) == 0) {
        flags |= 0x10;
        goto out;
    }
    owner = is_unit(x, y);
    if (owner >= 0 && g_cur_colony_rec->nation != owner && terrain_is_ocean(x, y) == 0 && owner < 4) {
        for (u = top_unit_in_square(x, y); u >= 0; u = next_unit(u)) {
            if ((g_unit_types[g_units[u].type].attack > 1
                 || (g_units[u].type == UNIT_PIONEERS && (unsigned char)(g_units[u].flags & 0xf) < 4
                     && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].control == 0))
                && (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)) {
                flags |= 0x80;
                spot_unit(u, g_cur_colony_rec->nation);
            }
        }
    }
    if (lost_city_at(x, y))
        flags |= 2;
    for (i = 0; g_game.settlement_count > i; i++) {
        if (g_settlements[i].x == (unsigned char)x && g_settlements[i].y == (unsigned char)y)
            flags |= 4;
    }
    for (i = 0; g_game.colony_count > i; i++) {
        if (i != g_cur_colony_idx) {
            if (g_colonies[i].x == (unsigned char)x && g_colonies[i].y == (unsigned char)y)
                flags |= 0x20;
            if (ABS(g_colonies[i].x - x) <= 2 && ABS(g_colonies[i].y - y) <= 2) {
                rdx = cx - (g_colonies[i].x - g_cur_colony_rec->x);
                rdy = cy - (g_colonies[i].y - g_cur_colony_rec->y);
                ring = field_index(rdx, rdy);
                if (ring >= 0 && (int)g_colonies[i].slots[ring] >= 0)
                    flags |= 0x40;
            }
        }
    }
    if (adx == 0 && ady == 0)
        flags |= 8;
out:
    return flags;
}
