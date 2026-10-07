/* set_course_for_europe: candidate for 1030:2c74 (618 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * SENDS A UNIT TO THE NEAREST SEA LANE. Rings of radius 1 upward are searched for a square of
 * terrain 0x1a that either nobody holds or this unit's own nation does: the top and bottom edges
 * first (one `x` loop whose `y` steps by `r * 2`, so it visits both), then the right edge, then
 * the left. The search stops at the first hit or when the radius passes the map width.
 *
 * On a hit the nation's counter at DGROUP:0x4b70 goes up -- the same counter
 * `foreign_unit` decrements for a unit on mission 0x45 -- the unit's destination
 * (+9, +0xa) becomes the square, and mission 0x45 is written at +7. A unit not already on orders
 * 2 also gets orders 3 for a human nation or 0xb otherwise, as a TERNARY: both arms load AL and
 * there is one store (rule 60).
 *
 * THE MATCH TEST IS AN EMPTY `if` WITH AN `else`: `if (n >= 0 && n != nation) ; else { ... }`.
 * Both the `||` form and `!(a && b)` compile to one jump past the body; the shipped jumps INTO
 * the body from both tests and then over it, which only the empty-then spelling gives.
 *
 * `y += r * 2` and not `r + r`: the shipped doubles in the register (`ADD AX,AX`) where the sum
 * of two reads adds from memory.
 *
 * RENAMED 2026-09-26 from unit_head_for_sea_lane: the Mac build's set_course_for_europe(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller orders_port; caller perform_orders_route; caller plan_move; callee is_unit), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "units.h"
#include "terrain.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern int is_unit(int x, int y);

void set_course_for_europe(int u)
{
    int x0;
    int y0;
    int r;
    int found;
    int fx;
    int fy;
    int n;
    int nation;
    register int y;
    register int x;

    found = 0;
    x0 = g_units[u].x;
    y0 = g_units[u].y;
    nation = NATION(g_units[u]);
    for (r = 1; found == 0 && g_map_size.w > r; r++) {
        for (x = x0 - r; found == 0 && x0 + r > x; x++) {
            for (y = y0 - r; found == 0 && y0 + r >= y; y += r * 2) {
                if (terrain_at(x, y) == TERRAIN_SEA_LANE) {
                    n = is_unit(x, y);
                    if (n >= 0 && n != nation)
                        ;
                    else {
                        found = 1;
                        fx = x;
                        fy = y;
                    }
                }
            }
        }
        x = x0 + r;
        for (y = y0 - r; found == 0 && y0 + r >= y; y++) {
            if (terrain_at(x, y) == TERRAIN_SEA_LANE) {
                n = is_unit(x, y);
                if (n >= 0 && n != nation)
                        ;
                    else {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
        x = x0 - r;
        for (y = y0 - r; found == 0 && y0 + r >= y; y++) {
            if (terrain_at(x, y) == TERRAIN_SEA_LANE) {
                n = is_unit(x, y);
                if (n >= 0 && n != nation)
                        ;
                    else {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
    }
    if (found != 0) {
        g_nation_ships_in_transit[NATION(g_units[u])]++;
        if (g_units[u].orders != ORDERS_TRADE_ROUTE)
            g_units[u].orders = NATION(g_units[u]) < 4
                                && g_nation_text[NATION(g_units[u])].control == CONTROL_HUMAN ? 3 : 0xb;
        g_units[u].dest_x = fx;
        g_units[u].dest_y = fy;
        g_units[u].mission = 0x45;
    }
}
