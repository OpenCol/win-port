/* count_plane1_around: candidate for 1018:646a (80 bytes).  Verdict: RELOCATABLE.
 * symbol: count_plane1_around
 *
 * Sums map_plane1_test_bits(x + dx, y + dy, bits) over the eight neighbours.
 * The helper is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

int count_plane1_around(int x, int y, int bits)
{
    register int n = 0;
    register int dir;

    for (dir = 0; dir < 8; dir++)
        n += map_plane1_test_bits(g_dir_dx[dir] + x, g_dir_dy[dir] + y, bits);
    return n;
}
