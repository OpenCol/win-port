/* square_region_set: candidate for 1038:ba66 (89 bytes).  Verdict: RELOCATABLE.
 *
 * The square's CONNECTED-REGION id := v -- the low nibble of the plane-2 byte at (x, y),
 * when the index is in range. `map_find_continents` is the caller that fills the map.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char *continent_loc(int x, int y);
void square_region_set(int x, int y, int region)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return;
    p = continent_loc(x, y);
    *p = (*p & 0xf0) | (unsigned char)(region & 0xf);
}
