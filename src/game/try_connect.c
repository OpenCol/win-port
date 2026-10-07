/* try_connect: candidate for 1018:0000 (770 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 770, not the census 761. The trailing 9 bytes are the dead second epilogue.
 *
 * For the current colony, looks for another colony of the same nation within seven squares on
 * either axis and on the same land region, and tries to send a unit there. The table at
 * SEG20:0xdab0, one row of 16 per nation and indexed by region, must read at least 2, and each
 * candidate colony is taken only when `rand_range(0, entry - 2)` comes up 0. For the one taken, a
 * path is walked square by square through 1020:b728 (mode 1, destination in DGROUP:0x4bf0/0x4bf2,
 * no moving nation), and the walk fails as soon as it steps on a square nobody owns whose plane-1 byte has
 * neither bit 1 nor bit 3 set. A walk that reaches the colony, or that 1020:b728 stops, leaves `ok`
 * set and the next colony is tried.
 *
 * When the walk fails, the square it stopped on must not belong to another nation by either test.
 * If the colony's signed byte +0x8c is at least that square's terrain value (row +4 of the terrain
 * table) plus 2, a unit of the type byte at SEG20:0x7ae6 is created there with byte +0x16 = 0x63,
 * given one mission step, and the scratch unit freed; the answer is then 1. Once a failed walk's square passes
 * the ownership tests the search stops, unit or not; a square of another nation sends it on to the
 * next colony.
 *
 * The walk's position lives in SI and DI and is never reset between candidates, so the second
 * colony's walk starts where the first one ended.
 *
 * RENAMED 2026-09-26 from colony_path_probe: the Mac build's try_connect(void), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller colony_plan; callee close_move; callee feature_get; callee is_anything; callee is_city), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "terrain.h"
#include "units.h"
#define ABS(d) ((d) > 0 ? (d) : ~(d) + 1)

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int land_region_at(int x, int y);

extern int close_move(int x, int y, int limit);
extern int is_city(int x, int y);
extern unsigned char feature_get(int x, int y);
extern int is_anything(int x, int y);
extern int is_hostile(int x, int y, int me);

extern int make_fake_unit(int icon, int x, int y, int owner, int strength);
extern void perform_orders_road(int u);
extern void blow_fake_unit(void);

int try_connect(void)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    int r;
    int i;
    int nation;
    int region;
    int region2;
    int cx;
    int cy;
    int dir;
    int ok;
    int owner;
    int u;
    int chance;
    int terr;
    int need;
    register int x;
    register int y;

    r = 0;
    x = g_cur_colony_rec->x;
    y = g_cur_colony_rec->y;
    region = land_region_at(x, y);
    nation = g_cur_colony_rec->nation;
    if (g_region_colony_count[nation][region] < 2)
        goto out;
    chance = g_region_colony_count[nation][region] - 2;
    for (i = 0; g_game.colony_count > i; i++) {
        if (g_colonies[i].nation != (unsigned char)nation)
            continue;
        if (g_cur_colony_idx == i)
            continue;
        cx = g_colonies[i].x;
        cy = g_colonies[i].y;
        if (ABS(cx - x) >= 7 && ABS(cy - y) >= 7)
            continue;
        region2 = land_region_at(cx, cy);
        if (region != region2)
            continue;
        if (rand_range(0, chance) != 0)
            continue;
        g_flat_step_cost = 1;
        g_moving_nation = -1;
        g_grid_x = cx;
        g_grid_y = cy;
        g_path_unit_type = 1;
        ok = 1;
        do {
            dir = close_move(x, y, 0x63);
            if (dir < 0 || dir == 8)
                break;
            x += g_dir_dx[dir];
            y += g_dir_dy[dir];
            if (x == cx && y == cy)
                break;
            if (is_city(x, y) < 0 && !(feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                ok = 0;
        } while (ok != 0);
        g_flat_step_cost = 0;
        if (ok != 0)
            continue;
        owner = is_anything(x, y);
        if (owner >= 0 && owner != nation)
            continue;
        owner = is_hostile(x, y, nation);
        if (owner >= 0 && owner != nation)
            continue;
        terr = terrain_at(x, y);
        need = g_terrain_rows[terr].work_turns + 2;
        if (g_cur_colony_rec->ai_idle_turns >= (signed char)need) {
            u = make_fake_unit(g_unit_types[UNIT_PIONEERS].icon, x, y, nation, 0);
            g_units[u].counter = 0x63;
            perform_orders_road(u);
            blow_fake_unit();
            r = 1;
        }
        break;
    }
out:
    return r;
}
