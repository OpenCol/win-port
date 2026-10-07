/* find_4: candidate for 1020:c0f2 (524 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 524, not the census 515. The trailing 9 bytes are the dead second epilogue.
 *
 * THE COARSE GRID. The map is divided into 4x4 blocks -- 15 across by 18 down -- and two byte
 * tables at SEG27:0xae and SEG27:0x1bc mark the blocks of the two kinds the `want` argument
 * chooses between. This finds the block to head for from (x, y): the unit's own block if it is
 * marked, otherwise the marked neighbour whose centre is the shortest octile distance away.
 *
 * A block only counts if it actually has a usable square: `get_valid_square` looks
 * inside the 2x2 at the block's centre (block * 4 + 1) and hands back a real square, and
 * `move_dist` must then accept it. The chosen block's coordinates are left in DGROUP:0x4bf0
 * and 0x4bf2, and the answer is whether one was found at all.
 *
 * `(want != 0 && marked_1bc) || (want == 0 && marked_ae)` is the shape the jumps insist on: the
 * first pair falls THROUGH to the second's test rather than skipping it, which an `if`/`else`
 * would not do. Both bounds checks are `continue`s in the two-jump form (rule 12).
 *
 * RENAMED 2026-09-26 from nearest_grid_dir: the Mac build's find_4(short, short, short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller far_move; callee get_valid_square; callee xy_dist), the runner-up gets 1; the arity agrees (3), and the body does what the Mac name says.
 */

#include "colonization.h"

extern int xy_dist(int dx, int dy);

/* same object: 4-byte `push cs; call ` */
extern int get_valid_square(int corner_x, int y0, int *px, int *py, int want);
extern int move_dist(int tx, int ty, int x, int y, int want, int limit);

int find_4(int x, int y, int want)
{
    int best_dir;
    int gx;
    int gy;
    int ny;
    int tx;
    int ty;
    int best;
    int dist;
    register int dir;
    register int nx;

    best_dir = -1;
    gx = x >> 2;
    gy = y >> 2;
    if (want != 0) {
        if (g_coarse_grid_sea[gx][gy] != 0)
            best_dir = 8;
    } else {
        if (g_coarse_grid_land[gx][gy] != 0)
            best_dir = 8;
    }
    if (best_dir == 8) {
        if (get_valid_square(gx * 4 + 1, gy * 4 + 1, &tx, &ty, want) == 0)
            best_dir = -1;
        else if (move_dist(tx, ty, x, y, want, 0x12) < 0)
            best_dir = -1;
    }
    if (best_dir < 0) {
        best = 0x63;
        for (dir = 0; dir < 8; dir++) {
            nx = g_dir_dx[dir] + gx;
            ny = g_dir_dy[dir] + gy;
            if (nx < 0 || nx >= 0xf)
                continue;
            if (ny < 0 || ny >= 0x12)
                continue;
            if ((want != 0 && g_coarse_grid_sea[nx][ny] != 0)
                || (want == 0 && g_coarse_grid_land[nx][ny] != 0)) {
                tx = nx * 4 + 1;
                ty = ny * 4 + 1;
                dist = xy_dist(x - tx, y - ty);
                if (dist < best) {
                    if (get_valid_square(tx, ty, &tx, &ty, want) != 0
                        && move_dist(tx, ty, x, y, want, 0x12) >= 0) {
                        best = dist;
                        best_dir = dir;
                    }
                }
            }
        }
    }
    if (best_dir >= 0) {
        g_grid_x = g_dir_dx[best_dir] + gx;
        g_grid_y = g_dir_dy[best_dir] + gy;
    }
    return best_dir >= 0;
}
