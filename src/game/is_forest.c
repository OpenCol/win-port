/* is_forest: candidate for 1040:43aa (87 bytes).  Verdict: RELOCATABLE.
 *
 * Low five bits of the plane-0 byte in 8..15 or 16..23: the forested terrain ids.
 */

#include "colonization.h"

#include "terrain.h"

extern unsigned char map_get(int x, int y);

int is_forest(int x, int y)
{
    int t = map_get(x, y);

    t = (unsigned char)(t & MAP0_TERRAIN_MASK);
    return (t >= TERRAIN_BOREAL_FOREST && t < TERRAIN_BOREAL_FOREST_2)
        || (t >= TERRAIN_BOREAL_FOREST_2 && t < TERRAIN_ARCTIC);
}
