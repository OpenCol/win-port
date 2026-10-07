/* adjacent_foreign_owner: candidate for 1038:28c9 (133 bytes).  Verdict: RELOCATABLE.
 *
 * Looks around (x, y) for a tile owned (is_unit) by a nation
 * other than `nation`; the owner goes to DGROUP:0x4c36 (-1 when none) and
 * the result says whether one was found.
 */

#include "colonization.h"

extern int is_unit(int x, int y);

int adjacent_foreign_owner(int x, int y, int nation)
{
    register int i;
    register int owner;
    int nx;
    int ny;

    g_foreign_owner = -1;
    for (i = 0; g_foreign_owner < 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        owner = is_unit(nx, ny);
        if (owner >= 0 && owner != nation)
            g_foreign_owner = owner;
    }
    return g_foreign_owner >= 0;
}
