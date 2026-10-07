/* island: candidate for 1020:026d (176 bytes).  Verdict: RELOCATABLE.
 * symbol: island
 * name-source: sibling-build
 *
 * The land brush. It sets one cell of the scratch plane, steps to a random ORTHOGONAL
 * neighbour and repeats, between 3 and 0x12 times: `rand_range(1, 4) * 2 - 2` gives 0, 2, 4 or
 * 6, the even entries of the eight-direction tables, which are N, E, S and W.
 *
 * Its diagonal counterparts are `blob` (the odd entries, a wider and
 * splashier brush) and `blob2` (diagonal with four one-in-four splashes).
 * Three brushes, one shape of loop, differing only in the step and the length.
 *
 * The walk stops at the working rectangle at DGROUP:0x4bd6..0x4bd9 rather than being clamped,
 * and `n-- != 0` is a post-decrement -- the counter is read, decremented in place and the value
 * that was read is what is tested.
 *
 * RENAMED 2026-09-27 from walk_orthogonal: the Mac build's island, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Orthogonal random-walk land brush: rand_range, buffer_put, rand_range, build_check is exactly Mac island's call sequence (random, buffer_put, random, build_check); 2 args, ratio 1.10; the address order walk_diagonal<random_walk_blob<walk_orthogonal matches blob<blob2<island.
 */

#include "colonization.h"

extern void buffer_put(short handle, int x, int y, char v);

extern int build_check(int flag);

void island(register int x, register int y)
{
    int n;
    int dir;

    n = rand_range(1, 0x10) + 2;
    while (n-- != 0 && g_x_lo < x && g_x_hi > x && g_y_lo < y && g_y_hi > y) {
        buffer_put(g_plane3_handle, x, y, 1);
        dir = rand_range(1, 4) * 2 - 2;
        x += g_dir_dx[dir];
        y += g_dir_dy[dir];
    }
    build_check(0);
}
