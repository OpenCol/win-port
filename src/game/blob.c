/* PORT: this file's own declarations of bloop removed: the header declares it from the definition (W1 step 3). */
/* blob: candidate for 1020:00a8 (161 bytes).  Verdict: RELOCATABLE.
 * symbol: blob
 * name-source: sibling-build
 *
 * The wide brush: `bloop`'s three-cell wedge dropped at every step of a
 * DIAGONAL random walk, between 3 and 0x42 times -- the longest of the three walks by a
 * factor of four over `island`.
 *
 * `rand_range(1, 4) * 2 - 1` gives 1, 3, 5 or 7, the odd entries of the direction tables. A
 * diagonal step and a three-cell mark together cover both colours of the checkerboard, which
 * a bare diagonal walk does not.
 *
 * RENAMED 2026-09-27 from walk_diagonal: the Mac build's blob, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Diagonal walk with one bloop per step, then build_check, matches Mac blob (random, bloop, random, build_check); 2 args, ratio 1.11.
 */

#include "colonization.h"

extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */

void blob(register int x, register int y)
{
    int n;
    int dir;

    n = rand_range(1, 0x40) + 2;
    while (n-- != 0 && g_x_lo < x && g_x_hi > x && g_y_lo < y && g_y_hi > y) {
        bloop(x, y);
        dir = rand_range(1, 4) * 2 - 1;
        x += g_dir_dx[dir];
        y += g_dir_dy[dir];
    }
    build_check(0);
}
