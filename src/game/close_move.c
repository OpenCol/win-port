/* close_move: candidate for 1020:b728 (2269 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2269, not the census 2260. The trailing 9 bytes are the dead second epilogue.
 *
 * THE SHORT-RANGE ROUTE, the counterpart of `far_move`. The target is the
 * square already in DGROUP:0x4bf0/0x4bf2; the answer is the direction to step from (x, y) toward it,
 * or -1. `move_to` calls it with limit 999 or 998, `move_dist`
 * and `try_connect` with their own. The cheapest cost found is left in DGROUP:0x4bf4.
 *
 * THE SEARCH works in a 16x16 window centred on the target. Move costs go into the byte grid at
 * SEG23:0x10e, the target costing 1, and the queue is the same 256-entry ring at SEG23:0x20e/0x30e
 * that the long route uses. It runs outward from the target until the queue empties or 0xe1 squares
 * have been taken off it; when (x, y) comes off it its cost becomes the result and the search
 * goes on. A square is
 * entered when it is inside the window and on the map, of the mover's element (land, or one of the
 * two ocean terrains in map-plane-2 region 1 for a ship), or is a European-owned square that is
 * either (x, y) or the target itself (0x4bfa/0x4bfc were just set to it). Land units (path modes 0x13 and up) avoid Lost City Rumour squares (`lost_city_at`).
 * With a moving nation set, squares another nation owns are closed, and a square `is_hostile` reports (land of another
 * European nation with pair flag 0x40) costs 8 more for a human nation and is closed to a computer one.
 *
 * THE STEP COST is 1 from a road to a road (plane 1 road or settlement bits), or for any step while
 * DGROUP:0x10a2 is set; 1 along a river (MAP0_RIVER) in a straight line; 3 for a unit type whose byte at
 * +4 is 3 or less; otherwise three times the terrain's byte at SEG20:0x78f0+0xa. A square keeps the
 * lowest cost that reaches it.
 *
 * THE CACHE. The target is remembered in DGROUP:0x4bfa/0x4bfc. Called again for the same target with
 * (x, y) already costed, the search is skipped and the result is left at 0 -- always under the limit.
 *
 * THE ANSWER: when the result is under `limit`, each neighbour of (x, y) that passes the same entry
 * tests and costs less than (x, y) itself is priced again with the step cost, and the cheapest wins;
 * ties go to the neighbour nearer the target by octile distance. With no winner the target reverts
 * to the remembered one.
 *
 * DEBUG, exactly as in the long route: with DGROUP:0x10a6 set, or debug bit 4 at DGROUP:0xb6 on and
 * the mover visible, the map is recentred on (x, y) and "(%d,%d)-(%d,%d) %d == %d" is formatted,
 * with a `Z`/`X` zoom loop whose key is never read.
 *
 * SHIPPED QUIRKS. The tie-break tests `best == best` -- always true; `ncost == best` was meant, and
 * happens to be implied by the enclosing `ncost <= best`. Two `top_unit_in_square` results are stored and never
 * read, one of them tested by an empty `if`. The head of the queue is not masked to 0xff the way the
 * tail is; the 0xe1 bound keeps it in range. Unlike `move_to`, the river test
 * here is correct: `(cx == nx || cy == ny)` both sit inside it.
 *
 * It graded RELOCATABLE on the first build. The distance tests are the `ABSDIFF` spelling from
 * `move_dist`, the zoom block is `far_move`'s verbatim, and the
 * pick loop mixes `continue`s (JCC over JMP) for the entry tests with a structured `if` for the cost
 * test, just as the shipped jumps do.
 *
 * RENAMED 2026-09-26 from near_path_step: the Mac build's close_move(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (callee continent_at; callee feature_get; callee is_city; callee is_colony; callee map_dist), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "dbgopt.h"
#include "terrain.h"
#define ABSDIFF(a, b) ((a) - (b) > 0 ? (a) - (b) : ~((a) - (b)) + 1)
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern unsigned char continent_at(int x, int y);

extern int is_colony(int x, int y);
extern int lost_city_at(int x, int y);
extern int is_anything(int x, int y);
extern int is_hostile(int x, int y, int me);
extern int is_city(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int map_dist(int x1, int y1, int x2, int y2);
extern void DrawMap(int full);

int close_move(int x, int y, int limit)
{
    int cx;
    int ship;
    int slow;
    int ox;
    int oy;
    int nx;
    int ny;
    int depth;
    int cost;
    int roads;
    int terrain;
    int best_dir;
    int best;
    int old;
    int ncost;
    int unit;
    int river;
    int owner;
    int show;
    int key;
    int step;
    int here;
    int px;
    int py;
    int t1;
    int t2;
    int t3;
    int t4;
    char buf[80];
    register int cy;
    register int dir;

    show = g_path_shown;
    if (show == 0 && (g_debug_flags & DBG_CLOSE_MOVES)
        && (g_game.reveal_all != 0 || g_game.view_nation == g_moving_nation))
        show = 1;
    ox = g_grid_x - 8;
    oy = g_grid_y - 8;
    ship = g_path_unit_type >= 13 && g_path_unit_type <= 18;
    slow = g_unit_types[g_path_unit_type].moves <= 3;
    g_path_result = 0;
    if (g_grid_x == g_near_last_x && g_grid_y == g_near_last_y
        && g_near_depth[x - ox][y - oy] != 0)
        goto pick;
    g_near_last_x = g_grid_x;
    g_near_last_y = g_grid_y;
    memset_rtl(g_near_depth, 0, 0x100);
    g_queue_tail = g_queue_head = 0;
    g_queue_x[g_queue_tail] = g_grid_x;
    g_queue_y[g_queue_tail] = g_grid_y;
    g_queue_tail++;
    g_near_depth[g_grid_x - ox][g_grid_y - oy] = 1;
    g_path_result = limit;
    do {
        cx = g_queue_x[g_queue_head];
        cy = g_queue_y[g_queue_head];
        g_queue_head++;
        depth = g_near_depth[cx - ox][cy - oy];
        if (depth > g_path_result)
            continue;
        if (cx == x && cy == y) {
            g_path_result = depth;
            continue;
        }
        roads = feature_get(cx, cy) & (MAP1_SETTLEMENT | MAP1_ROAD);
        river = map_get(cx, cy) & MAP0_RIVER;
        for (dir = 0; dir < 8; dir++) {
            nx = g_dir_dx[dir] + cx;
            ny = g_dir_dy[dir] + cy;
            if (ABSDIFF(nx, g_grid_x) >= 8)
                continue;
            if (ABSDIFF(ny, g_grid_y) >= 8)
                continue;
            if (!on_map(nx, ny))
                continue;
            terrain = terrain_at(nx, ny);
            if (!((((terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) ? 1 : 0) == ship
                   && (ship == 0 || continent_at(nx, ny) == 1))
                  || (is_colony(nx, ny) >= 0
                      && ((nx == x && ny == y) || (nx == g_near_last_x && ny == g_near_last_y)))))
                continue;
            if (g_path_unit_type >= 0x13 && lost_city_at(nx, ny))
                continue;
            cost = depth;
            if (g_moving_nation >= 0) {
                owner = is_anything(nx, ny);
                if (owner >= 0 && owner != g_moving_nation)
                    continue;
                if (is_hostile(nx, ny, g_moving_nation) >= 0) {
                    if (g_moving_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_moving_nation].control == CONTROL_HUMAN)
                        cost += 8;
                    else
                        continue;
                }
            }
            if (g_moving_nation >= 0 && owner == g_moving_nation
                && (owner >= NATION_EUROPEAN_COUNT || g_nation_text[owner].control != CONTROL_HUMAN)
                && is_city(nx, ny) < 0) {
                unit = top_unit_in_square(nx, ny);
                if (unit >= 0)
                    ;
            }
            if ((roads != 0 && (feature_get(nx, ny) & (MAP1_SETTLEMENT | MAP1_ROAD))) || g_flat_step_cost != 0)
                cost++;
            else if (river != 0 && (map_get(nx, ny) & MAP0_RIVER) && (cx == nx || cy == ny))
                cost++;
            else if (slow != 0)
                cost += 3;
            else
                cost += g_terrain_rows[terrain].movement * 3;
            old = g_near_depth[nx - ox][ny - oy];
            if (old == 0 || old > cost) {
                g_near_depth[nx - ox][ny - oy] = cost;
                g_queue_x[g_queue_tail] = nx;
                g_queue_y[g_queue_tail] = ny;
                g_queue_tail = (g_queue_tail + 1) & 0xff;
            }
        }
    } while (g_queue_head != g_queue_tail && g_queue_head < 0xe1);
pick:
    best_dir = -1;
    if (g_path_result < limit) {
        best = 0x63;
        depth = 0x63;
        roads = feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD);
        river = map_get(x, y) & MAP0_RIVER;
        here = g_near_depth[x - ox][y - oy];
        for (dir = 0; dir < 8; dir++) {
            nx = g_dir_dx[dir] + x;
            ny = g_dir_dy[dir] + y;
            if (ABSDIFF(nx, g_grid_x) >= 8)
                continue;
            if (ABSDIFF(ny, g_grid_y) >= 8)
                continue;
            if (!on_map(nx, ny))
                continue;
            terrain = terrain_at(nx, ny);
            if (!((((terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) ? 1 : 0) == ship
                   && (ship == 0 || continent_at(nx, ny) == 1))
                  || (is_colony(nx, ny) >= 0
                      && ((nx == x && ny == y) || (nx == g_near_last_x && ny == g_near_last_y)))))
                continue;
            if (g_path_unit_type >= 0x13 && lost_city_at(nx, ny))
                continue;
            ncost = g_near_depth[nx - ox][ny - oy];
            if (g_moving_nation >= 0) {
                owner = is_anything(nx, ny);
                if (owner >= 0 && owner != g_moving_nation)
                    continue;
                if (is_hostile(nx, ny, g_moving_nation) >= 0) {
                    if (g_moving_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_moving_nation].control == CONTROL_HUMAN)
                        ncost += 8;
                    else
                        continue;
                }
            }
            if (ncost != 0 && ncost < here) {
                if (roads != 0 && (feature_get(nx, ny) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                    step = 1;
                else if (river != 0 && (map_get(nx, ny) & MAP0_RIVER) && (x == nx || y == ny))
                    step = 1;
                else if (slow != 0)
                    step = 3;
                else
                    step = g_terrain_rows[terrain_at(nx, ny)].movement * 3;
                ncost += step;
                if (ncost <= best) {
                    unit = top_unit_in_square(nx, ny);
                    cost = map_dist(g_grid_x, g_grid_y, nx, ny);
                    if (ncost < best) {
                        best = ncost;
                        best_dir = dir;
                        depth = cost;
                    } else if (best == best && cost < depth) {
                        best_dir = dir;
                        depth = cost;
                    }
                }
            }
        }
    }
    if (best_dir < 0) {
        g_grid_x = g_near_last_x;
        g_grid_y = g_near_last_y;
    }
    if (show) {
again:
        g_map_center_x = x;
        g_map_center_y = y;
        DrawMap(1);
        for (cx = 0; cx < 16; cx++) {
            px = cx + ox;
            for (cy = 0; cy < 16; cy++)
                py = cy + oy;
        }
        sprintf_rtl(buf, "(%d,%d)-(%d,%d) %d == %d", g_grid_x, g_grid_y, x, y, limit, best_dir);
        switch (key) {
        case 'Z':
            g_zoom = *max_ptr_1020_e7e6((t2 = 0, &t2), (t1 = g_zoom - 1, &t1));
            goto again;
        case 'X':
            g_zoom = *min_ptr_1020_e818((t4 = 3, &t4), (t3 = g_zoom + 1, &t3));
            goto again;
        }
        DrawMap(1);
    }
    return best_dir;
}
