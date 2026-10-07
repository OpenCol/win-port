/* colony_tile_nibble: candidate for 1018:3c4b (70 bytes).  Verdict: RELOCATABLE.
 *
 * The plane-2 nibble of colony c's own tile (land_region_at at
 * its x, y).
 */

#include "colonization.h"

#include "records.h"

extern int land_region_at(int x, int y);

int colony_tile_nibble(int colony)
{
    return land_region_at(g_colonies[colony].x, g_colonies[colony].y);
}
