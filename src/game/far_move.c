/* far_move: candidate for 1020:c2fe (1333 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1333, not the census 1324. The trailing 9 bytes are the dead second epilogue.
 *
 * THE LONG-RANGE ROUTE, for `1020:c833`. It works on the coarse grid of 4x4 blocks
 * (see `find_4`) and answers whether the block holding (sx, sy) is connected to the
 * block holding (tx, ty). In either case it leaves a square to head for in DGROUP:0x4bf0/0x4bf2.
 *
 * Both ends are first moved onto a usable block by `find_4`. The start's block is also saved at
 * DGROUP:0x4bfe/0x4c00. `ship` is whether the path mode at DGROUP:0x10a0 is a ship type (13..18); it
 * selects the SEG27:0x1bc links over the SEG27:0xae ones. Each block's byte in those tables is a mask of
 * the eight directions it connects in.
 *
 * THE SEARCH is breadth-first FROM THE TARGET. Depths go into the 15x18 byte grid at SEG23:0x0 (target = 1),
 * and the ring queue of 256 block coordinates is at SEG23:0x20e/0x30e, with the tail at DGROUP:0x4bf6 and
 * the head at 0x4bf8. It stops when the start block comes off the queue.
 *
 * THE STEP: among the start block's connected neighbours that the search reached, the lowest depth wins.
 * On a tie, the neighbour whose centre is nearer the target by octile distance wins. The waypoint is a real
 * square found that neighbour's centre by `get_valid_square`. With no such neighbour, or
 * no route, the waypoint is the target itself.
 *
 * DEBUG: when DGROUP:0x10a8 is set, or debug bit 5 at DGROUP:0xb6 is on and the mover is visible, the map
 * is recentred on the start block and a "Far: ..." line is formatted. A `Z`/`X` key zooms out or in and
 * repeats. The keyboard read is gone from the shipped code, so `key` is never written, and the formatted
 * line and the 15x18 loop do nothing. On exit DGROUP:0x10a6 notes that the debug view was shown.
 *
 * `mask` is a ternary (the join comes before the `MOV AH,0`). The second scan tests `(mask & bit) == 0`
 * and `continue`s (rule 12), while the first wraps its body in an `if`. The zoom clamps use rule 61's
 * comma operands.
 *
 * RENAMED 2026-09-26 from far_path_waypoint: the Mac build's far_move(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller move_to; callee map_dist), the runner-up gets 1, and the arity agrees (4). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "dbgopt.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int map_dist(int x1, int y1, int x2, int y2);
extern void DrawMap(int full);

/* same object: 4-byte `push cs; call ` */
extern int get_valid_square(int corner_x, int y0, int *px, int *py, int want);
extern int find_4(int x, int y, int want);

int far_move(int sx, int sy, int tx, int ty)
{
    int found;
    int ship;
    int depth;
    int dist;
    int nx;
    int ny;
    int best_dir;
    int mask;
    int dir;
    int best;
    int nbr_depth;
    int have;
    int px;
    int py;
    int ox;
    int oy;
    int show;
    int key;
    int t1;
    int t2;
    int t3;
    int t4;
    char buf[80];
    register int cx;
    register int cy;

    found = 0;
    show = g_path_debug;
    if (show == 0 && (g_debug_flags & DBG_FAR_MOVES)
        && (g_game.reveal_all != 0 || g_game.view_nation == g_moving_nation))
        show = 1;
    g_grid_x = tx;
    g_grid_y = ty;
    ship = g_path_unit_type >= 13 && g_path_unit_type <= 18;
    if (find_4(sx, sy, ship)) {
        sx = g_far_start_x = g_grid_x;
        sy = g_far_start_y = g_grid_y;
    } else {
        g_grid_x = tx;
        g_grid_y = ty;
        goto out;
    }
    find_4(tx, ty, ship);
    if (g_grid_x >= 15 || g_grid_y >= 18)
        goto out;
    memset_rtl(g_grid_depth, 0, 0x10e);
    g_queue_tail = 0;
    g_queue_head = 0;
    g_queue_x[g_queue_tail] = g_grid_x;
    g_queue_y[g_queue_tail] = g_grid_y;
    g_queue_tail++;
    g_grid_depth[g_grid_x][g_grid_y] = 1;
    do {
        cx = g_queue_x[g_queue_head];
        cy = g_queue_y[g_queue_head];
        g_queue_head = (g_queue_head + 1) & 0xff;
        if (cx == sx && cy == sy) {
            found = 1;
            break;
        }
        depth = g_grid_depth[cx][cy];
        mask = ship ? g_coarse_grid_sea[cx][cy] : g_coarse_grid_land[cx][cy];
        for (dir = 0; dir < 8; dir++) {
            if (mask & (1 << dir)) {
                nx = g_dir_dx[dir] + cx;
                ny = g_dir_dy[dir] + cy;
                if (g_grid_depth[nx][ny] == 0) {
                    g_grid_depth[nx][ny] = depth + 1;
                    g_queue_x[g_queue_tail] = nx;
                    g_queue_y[g_queue_tail] = ny;
                    g_queue_tail = (g_queue_tail + 1) & 0xff;
                }
            }
        }
    } while (found == 0 && g_queue_tail != g_queue_head);
    have = 0;
    if (found) {
        best = 0x63;
        best_dir = -1;
        mask = ship ? g_coarse_grid_sea[sx][sy] : g_coarse_grid_land[sx][sy];
        for (dir = 0; dir < 8; dir++) {
            if ((mask & (1 << dir)) == 0)
                continue;
            nx = g_dir_dx[dir] + sx;
            ny = g_dir_dy[dir] + sy;
            nbr_depth = g_grid_depth[nx][ny];
            if (nbr_depth != 0) {
                if (nbr_depth < best) {
                    best = nbr_depth;
                    best_dir = dir;
                    depth = map_dist(tx, ty, (nx << 2) + 1, (ny << 2) + 1);
                } else if (nbr_depth == best) {
                    dist = map_dist(tx, ty, (nx << 2) + 1, (ny << 2) + 1);
                    if (dist < depth) {
                        best_dir = dir;
                        depth = dist;
                    }
                }
            }
        }
        if (best_dir >= 0) {
            px = ((g_dir_dx[best_dir] + sx) << 2) + 1;
            py = ((g_dir_dy[best_dir] + sy) << 2) + 1;
            ox = px;
            oy = py;
            have = 1;
            get_valid_square(px, py, &px, &py, ship);
        }
    }
    if (have) {
        g_grid_x = px;
        g_grid_y = py;
    } else {
        g_grid_x = tx;
        g_grid_y = ty;
    }
    if (show) {
again:
        g_map_center_x = sx;
        g_map_center_y = sy;
        DrawMap(1);
        for (cy = 0; cy < 18; cy++)
            for (cx = 0; cx < 15; cx++)
                ;
        sprintf_rtl(buf, "Far: %d(%d,%d) to (%d,%d) / (%d, %d) off (%d, %d)",
                    best_dir, g_dir_dx[best_dir], g_dir_dy[best_dir], px, py, ox, oy, sx << 2, sy << 2);
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
out:
    if (show)
        g_path_shown = 1;
    return found;
}
