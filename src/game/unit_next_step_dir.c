/* unit_next_step_dir -- shipped at 1020:c833.
 *
 * Ported by tools/port.py from matched/game/unit_next_step_dir.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_terrain_row_bytes[][0x10]; SEG20:0x78f0
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int far g_debug_flags;                        DGROUP:0x00b6
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern unsigned char far g_view_nation;              SEG20:0x7798
 *   extern int g_path_mode;                              DGROUP:0x10a0
 *   extern int g_path_shown;                             DGROUP:0x10a6
 *   extern int g_path_debug;                             DGROUP:0x10a8
 *   extern int g_grid_x;                                 DGROUP:0x4bf0
 *   extern int g_grid_y;                                 DGROUP:0x4bf2
 *   extern int g_far_start_x;                            DGROUP:0x4bfe
 *   extern int g_far_start_y;                            DGROUP:0x4c00
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 direction_of(col_i16 dx, col_i16 dy);
col_i16 far_path_waypoint(col_i16 sx, col_i16 sy, col_i16 tx, col_i16 ty);
col_i16 find_adjacent_tile(col_i16 x, col_i16 y, void *px, void *py, col_i16 want);
col_i16 near_path_step(col_i16 x, col_i16 y, col_i16 k);

#include "control.h"
#include "dbgopt.h"
#include "units.h"
#include "terrain.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
#define NATION(u) ((unsigned char)((u).flags & 0xf))















extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 square_nation_any(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);
extern unsigned char map_plane1_at(col_i16 x, col_i16 y);
extern unsigned char square_region(col_i16 x, col_i16 y);
extern unsigned char unit_move_points(col_i16 u);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 tile_claim(col_i16 x, col_i16 y, col_i16 n);
extern void ship_take_aboard(col_i16 u);
extern void unit_chain_relink(col_i16 u, col_i16 x, col_i16 y);
extern void unit_refresh_move_points(col_i16 u);

/* same object: 4-byte `push cs; call ` */





col_i16 unit_next_step_dir(col_i16 u)
{
    col_i16 dir;
    col_i16 nation;
    col_i16 dx;
    col_i16 dy;
    col_i16 x;
    col_i16 y;
    col_i16 tx;
    col_i16 ty;
    col_i16 adx;
    col_i16 ady;
    col_i16 retry;
    col_i16 human;
    col_i16 ship;
    col_i16 dist0;
    col_i16 best;
    col_i16 roads;
    col_i16 claim;
    col_i16 d;
    col_i16 ndx;
    col_i16 ndy;
    col_i16 nadx;
    col_i16 nady;
    col_i16 ndist;
    col_i16 terrain;
    col_i16 owner;
    col_i16 score;
    col_i16 river;
    col_i16 tries;
    col_i16 show;
    register col_i16 nx;
    register col_i16 ny;

    dir = -1;
    show = 0;
    if (g_debug_flags & DBG_ALL_MOVEMENT) {
        if (g_reveal_all != 0 || NATION(g_units[u]) == g_view_nation)
            show = 1;
    }
    tx = g_units[u].dest_x;
    ty = g_units[u].dest_y;
    x = g_units[u].x;
    y = g_units[u].y;
    g_path_mode = g_units[u].type;
    ship = g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST;
    if (!on_map(tx, ty))
        goto out;
    if (tx == x && ty == y)
        goto out;
    nation = NATION(g_units[u]);
    human = g_nation52[nation].control == CONTROL_HUMAN;
    dx = tx - x;
    dy = ty - y;
    if ((dx > 0 ? dx : ~dx + 1) <= 1 && (dy > 0 ? dy : ~dy + 1) <= 1) {
        dir = direction_of(dx, dy);
        goto out;
    }
    retry = 0;
    if ((tx - x > 0 ? tx - x : ~(tx - x) + 1) < 7
        && (ty - y > 0 ? ty - y : ~(ty - y) + 1) < 7) {
        g_grid_x = tx;
        g_grid_y = ty;
        dir = near_path_step(x, y, 0x3e7);
        if (dir >= 0)
            goto out;
        retry = 1;
    }
    if (far_path_waypoint(x, y, tx, ty) != 0 || retry == 0) {
        dir = near_path_step(x, y, 0x3e6);
        if (dir < 0) {
            g_grid_x = (g_far_start_x << 2) + 1;
            g_grid_y = (g_far_start_y << 2) + 1;
            find_adjacent_tile(g_grid_x, g_grid_y, &g_grid_x, &g_grid_y, ship);
            dir = near_path_step(x, y, 0x3e6);
        }
        if (dir >= 0) {
            if (g_units[u].moves != 0 && g_units[u].orders == ORDERS_MISSION
                && (col_i16)g_units[u].mission != 0x39
                && (col_i16)g_units[u].last_dir >= 0
                && ((col_i16)g_units[u].last_dir ^ 4) == dir) {
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
            ship_take_aboard(u);
            unit_chain_relink(u, g_units[u].dest_x, g_units[u].dest_y);
        }
        unit_refresh_move_points(u);
        goto out;
    }
    best = 0x270f;
    dir = 8;
    roads = map_plane1_at(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD);
    river = map_plane0_at(x, y) & MAP0_RIVER;
    claim = tile_claim(x, y, nation);
    for (d = 0; d < 8; d++) {
        nx = g_dir_dx[d] + x;
        ny = g_dir_dy[d] + y;
        ndx = dx - g_dir_dx[d];
        ndy = dy - g_dir_dy[d];
        nadx = ndx > 0 ? ndx : ~ndx + 1;
        nady = ndy > 0 ? ndy : ~ndy + 1;
        ndist = (nadx > nady ? nadx : nady) + (nadx + nady);
        if (human != 0 && ndist > dist0)
            continue;
        if (!on_map(nx, ny))
            continue;
        terrain = terrain_at(nx, ny);
        owner = square_nation_any(nx, ny);
        if (((owner < 0 || owner == nation)
             && ((terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) ? 1 : 0) == ship
             && (ship == 0 || square_region(nx, ny) == 1))
            || (square_european_nation(nx, ny) == nation && nx == tx && ny == ty)) {
            if (roads != 0 && (map_plane1_at(nx, ny) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                score = 1;
            else if ((river != 0 && (map_plane0_at(nx, ny) & MAP0_RIVER) && x == nx) || y == ny)
                score = 1;
            else
                score = unit_move_points(u) > 1 ? g_terrain_row_bytes[terrain][0xa] * 3 : 3;
            score = (ndist << 2) + (score + nadx + nady);
            if (score < best) {
                dir = d;
                best = score;
            }
        }
    }
    if ((col_i16)g_units[u].last_dir >= 0 && ((col_i16)g_units[u].last_dir ^ 4) == dir) {
        dir = -1;
        if (g_units[u].orders == ORDERS_MISSION && (col_i16)g_units[u].mission != 0x39) {
            for (tries = 0; dir < 0 && tries < 8; tries++) {
                dir = rand_range(0, 7);
                nx = g_dir_dx[dir] + x;
                ny = g_dir_dy[dir] + y;
                owner = square_nation_any(nx, ny);
                if (!((owner < 0 || owner == nation)
                      && ocean(nx, ny) == ship
                      && (ship == 0 || square_region(nx, ny) == 1)
                      && on_map(nx, ny)))
                    dir = -1;
            }
        }
        if (dir < 0)
            unit_refresh_move_points(u);
    }
    if (dir == 8) {
        unit_refresh_move_points(u);
        dir = -1;
    }
out:
    g_path_shown = 0;
    g_path_debug = 0;
    g_units[u].last_dir = dir;
    return dir;
}
