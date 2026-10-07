/* blob2: candidate for 1020:0149 (292 bytes).  Verdict: RELOCATABLE.
 * symbol: blob2
 * name-source: sibling-build
 *
 * A map-generator brush: it drops `bloop` on the square it is standing on, then on
 * each of the four DIAGONAL neighbours with probability one in four apiece, then steps to a
 * random diagonal neighbour and does it again -- between 3 and 0x32 times.
 *
 * THE STEP IS ALWAYS DIAGONAL. `rand_range(1, 4) * 2 - 1` gives 1, 3, 5 or 7, which are the
 * four odd entries of the eight-direction tables at SEG27:0x0 and SEG27:0x9 -- the corners.
 * The brush therefore travels on one colour of the checkerboard and fills the other colour in
 * only through the four one-in-four splashes.
 *
 * The walk also stops at the generator's working rectangle, four signed bytes at
 * DGROUP:0x4bd6..0x4bd9, tested STRICTLY -- a brush that reaches the edge stops rather than
 * being clamped to it, so the border stays blank.
 *
 * `n-- != 0` is the count: the shipped reads the counter, decrements it in place and tests the
 * value it read, which is the post-decrement and not the pre.
 *
 * RENAMED 2026-09-27 from random_walk_blob: the Mac build's blob2, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Diagonal walk with four one-in-four splashes: five bloop calls with rand_range tests, then build_check, matches Mac blob2 (random/bloop x5, build_check); 2 args, ratio 1.12; bloop is bloop.
 */

#include "colonization.h"

extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void bloop(int x, int y);

void blob2(register int x, register int y)
{
    int n;
    int dir;

    n = rand_range(1, 0x30) + 2;
    while (n-- != 0 && g_x_lo < x && g_x_hi > x && g_y_lo < y && g_y_hi > y) {
        bloop(x, y);
        if (rand_range(1, 4) == 1)
            bloop(x + 1, y + 1);
        if (rand_range(1, 4) == 1)
            bloop(x - 1, y + 1);
        if (rand_range(1, 4) == 1)
            bloop(x + 1, y - 1);
        if (rand_range(1, 4) == 1)
            bloop(x - 1, y - 1);
        dir = rand_range(1, 4) * 2 - 1;
        x += g_dir_dx[dir];
        y += g_dir_dy[dir];
    }
    build_check(0);
}
