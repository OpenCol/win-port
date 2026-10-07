/* terrain_tile_index: candidate for 1040:1408 (92 bytes).  Verdict: EXACT.
 *
 * The tile-set row for a terrain byte: the low five bits modulo 8 for the
 * land terrains (0..24), 0x24 for 25 (ocean) and 0x28 for 26.
 */

#include "colonization.h"

char terrain_tile_index(int terrain)
{
    char r = 0;
    register unsigned char c = terrain & 0x1f;

    if (c <= 0x18)
        r = c % 8;
    else if (c == 0x18)
        r = 0x20;
    else if (c == 0x19)
        r = 0x24;
    else if (c == 0x1a)
        r = 0x28;
    return r;
}
