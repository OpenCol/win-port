/* move_to: candidate for 1020:c833 (1925 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1925, not the census 1916. The trailing 9 bytes are the dead second epilogue.
 *
 * WHICH WAY A UNIT STEPS NEXT, the driver over `far_move` and `close_move`.
 * `perform_orders_move_to` and `1030:84d8` call it. The answer is a direction 0..7, or -1 when
 * the unit cannot or should not move; it is also stored back in the unit's +0xb.
 *
 * THE EASY ANSWERS come first. Off-map or already-there destinations answer -1. A destination
 * within one square answers `which_move` outright. Within seven squares each way the
 * short search `close_move(x, y, 999)` is tried against the destination; when it succeeds that
 * is the answer, and when it fails `retry` remembers that the long route is still worth a look.
 *
 * THE LONG ROUTE runs `far_move`, which leaves a waypoint in DGROUP:0x4bf0/0x4bf2,
 * and then searches to it with `close_move(x, y, 998)`. Failing that, the waypoint is moved to the
 * centre of the start block (`g_far_start_x * 4 + 1`) and `get_valid_square` pulls it onto
 * a real square, and the search is repeated. A direction that comes back is accepted -- unless the unit
 * is on standing orders 0xb and the step is the exact reverse (`^ 4`) of the one it took last time, in
 * which case the waypoint is forced to the true destination and the greedy step below runs instead.
 *
 * THE GREEDY STEP is for the four European nations only. It scores the eight neighbours of the square
 * against the waypoint and keeps the cheapest. A neighbour must be on the map, unowned or this nation's,
 * and of the right element -- land for a land unit, TERRAIN_OCEAN or TERRAIN_SEA_LANE for a ship,
 * which must also stand in map-plane-2 region 1 -- or else be the destination square itself in this
 * nation's territory. A computer nation may also try squares that do not close the distance; a human's
 * may not. The cost is one for a square that continues a road (plane 1 road or settlement bits) or a
 * river (plane 0 MAP0_RIVER) the unit is already on, three for a unit of rating 1 or less, and otherwise three
 * times the terrain's byte at SEG20:0x78f0+0xa; four times the remaining distance is added to it.
 *
 * ARRIVAL, when the waypoint is the square itself: orders are cleared, and a computer-owned ship
 * (types 13..18) unloads with `stack_ship` and relinks its stack at the destination.
 *
 * THE SHUFFLE. If the chosen step is again the reverse of the last one, the unit is stuck: eight random
 * directions are drawn and the first passable one is taken, giving up (-1) if none is. A direction of 8,
 * meaning "nothing beat staying put", also becomes -1. Either way the unit's rating is refreshed.
 *
 * SHIPPED QUIRKS. The river test reads `(river && plane0 & MAP0_RIVER && x == nx) || y == ny` -- the
 * `y == ny` arm is outside the river test, so every neighbour on the same row scores one whether there is
 * a river or not; `(x == nx || y == ny)` was surely meant. `claim` (`zoc`) and `show` (the debug
 * visibility at DGROUP:0xb6 bit 6) are both computed and never read.
 *
 * WHAT THE BYTES SETTLED: the waypoint call is a structured `if (... != 0 || retry == 0)`, not a `goto`
 * -- the shipped 386 long conditional jump is the tell (rule 12). The cost's last arm is a ternary, not
 * an `else`: both halves leave the value in AX and one store follows the join. Every absolute value is
 * rule 11's `~x + 1`, and the two `< 7` tests recompute `tx - x` rather than reuse `dx`. `nx` and `ny`
 * are the two `register` locals, SI and DI in declaration order (rule 53); the four same-segment callees
 * come through the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from unit_next_step_dir: the Mac build's move_to(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller indian_move; callee continent_at; callee end_unit_turn; callee feature_get; callee is_colony), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "dbgopt.h"
#include "units.h"
#include "terrain.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern int terrain_is_ocean(int x, int y);
extern int is_anything(int x, int y);
extern int is_colony(int x, int y);
extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern unsigned char continent_at(int x, int y);
extern unsigned char move_rate(int u);

extern int zoc(int x, int y, int n);
extern void stack_ship(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void end_unit_turn(int u);

/* same object: 4-byte `push cs; call ` */
extern int which_move(int dx, int dy);
extern int close_move(int x, int y, int limit);
extern int get_valid_square(int corner_x, int y0, int *px, int *py, int want);
extern int far_move(int sx, int sy, int tx, int ty);

int move_to(int u)
{
    int dir;
    int nation;
    int dx;
    int dy;
    int x;
    int y;
    int tx;
    int ty;
    int adx;
    int ady;
    int retry;
    int human;
    int ship;
    int dist0;
    int best;
    int roads;
    int claim;
    int cand_dir;
    int ndx;
    int ndy;
    int nadx;
    int nady;
    int ndist;
    int terrain;
    int owner;
    int score;
    int river;
    int tries;
    int show;
    register int nx;
    register int ny;

    dir = -1;
    show = 0;
    if (g_debug_flags & DBG_ALL_MOVEMENT) {
        if (g_game.reveal_all != 0 || NATION(g_units[u]) == ((unsigned char)g_game.view_nation))
            show = 1;
    }
    tx = g_units[u].dest_x;
    ty = g_units[u].dest_y;
    x = g_units[u].x;
    y = g_units[u].y;
    g_path_unit_type = g_units[u].type;
    ship = g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST;
    if (!on_map(tx, ty))
        goto out;
    if (tx == x && ty == y)
        goto out;
    nation = NATION(g_units[u]);
    human = g_nation_text[nation].control == CONTROL_HUMAN;
    dx = tx - x;
    dy = ty - y;
    if ((dx > 0 ? dx : ~dx + 1) <= 1 && (dy > 0 ? dy : ~dy + 1) <= 1) {
        dir = which_move(dx, dy);
        goto out;
    }
    retry = 0;
    if ((tx - x > 0 ? tx - x : ~(tx - x) + 1) < 7
        && (ty - y > 0 ? ty - y : ~(ty - y) + 1) < 7) {
        g_grid_x = tx;
        g_grid_y = ty;
        dir = close_move(x, y, 0x3e7);
        if (dir >= 0)
            goto out;
        retry = 1;
    }
    if (far_move(x, y, tx, ty) != 0 || retry == 0) {
        dir = close_move(x, y, 0x3e6);
        if (dir < 0) {
            g_grid_x = (g_far_start_x << 2) + 1;
            g_grid_y = (g_far_start_y << 2) + 1;
            get_valid_square(g_grid_x, g_grid_y, &g_grid_x, &g_grid_y, ship);
            dir = close_move(x, y, 0x3e6);
        }
        if (dir >= 0) {
            if (g_units[u].moves_used != 0 && g_units[u].orders == ORDERS_MISSION
                && (int)g_units[u].mission != 0x39
                && (int)g_units[u].last_dir >= 0
                && ((int)g_units[u].last_dir ^ 4) == dir) {
                g_grid_x = tx;
                g_grid_y = ty;
            } else
                goto out;
        }
    }
    if (NATION(g_units[u]) >= 4)
        goto out;
    dx = g_grid_x - x;
    dy = g_grid_y - y;
    adx = dx > 0 ? dx : ~dx + 1;
    ady = dy > 0 ? dy : ~dy + 1;
    dist0 = (adx > ady ? adx : ady) + (adx + ady);
    if (dx == 0 && dy == 0) {
        g_units[u].orders = ORDERS_NONE;
        if (human == 0 && g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            stack_ship(u);
            relocate_stacked_units(u, g_units[u].dest_x, g_units[u].dest_y);
        }
        end_unit_turn(u);
        goto out;
    }
    best = 0x270f;
    dir = 8;
    roads = feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD);
    river = map_get(x, y) & MAP0_RIVER;
    claim = zoc(x, y, nation);
    for (cand_dir = 0; cand_dir < 8; cand_dir++) {
        nx = g_dir_dx[cand_dir] + x;
        ny = g_dir_dy[cand_dir] + y;
        ndx = dx - g_dir_dx[cand_dir];
        ndy = dy - g_dir_dy[cand_dir];
        nadx = ndx > 0 ? ndx : ~ndx + 1;
        nady = ndy > 0 ? ndy : ~ndy + 1;
        ndist = (nadx > nady ? nadx : nady) + (nadx + nady);
        if (human != 0 && ndist > dist0)
            continue;
        if (!on_map(nx, ny))
            continue;
        terrain = terrain_at(nx, ny);
        owner = is_anything(nx, ny);
        if (((owner < 0 || owner == nation)
             && ((terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) ? 1 : 0) == ship
             && (ship == 0 || continent_at(nx, ny) == 1))
            || (is_colony(nx, ny) == nation && nx == tx && ny == ty)) {
            if (roads != 0 && (feature_get(nx, ny) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                score = 1;
            else if ((river != 0 && (map_get(nx, ny) & MAP0_RIVER) && x == nx) || y == ny)
                score = 1;
            else
                score = move_rate(u) > 1 ? g_terrain_rows[terrain].movement * 3 : 3;
            score = (ndist << 2) + (score + nadx + nady);
            if (score < best) {
                dir = cand_dir;
                best = score;
            }
        }
    }
    if ((int)g_units[u].last_dir >= 0 && ((int)g_units[u].last_dir ^ 4) == dir) {
        dir = -1;
        if (g_units[u].orders == ORDERS_MISSION && (int)g_units[u].mission != 0x39) {
            for (tries = 0; dir < 0 && tries < 8; tries++) {
                dir = rand_range(0, 7);
                nx = g_dir_dx[dir] + x;
                ny = g_dir_dy[dir] + y;
                owner = is_anything(nx, ny);
                if (!((owner < 0 || owner == nation)
                      && terrain_is_ocean(nx, ny) == ship
                      && (ship == 0 || continent_at(nx, ny) == 1)
                      && on_map(nx, ny)))
                    dir = -1;
            }
        }
        if (dir < 0)
            end_unit_turn(u);
    }
    if (dir == 8) {
        end_unit_turn(u);
        dir = -1;
    }
out:
    g_path_shown = 0;
    g_path_debug = 0;
    g_units[u].last_dir = dir;
    return dir;
}
