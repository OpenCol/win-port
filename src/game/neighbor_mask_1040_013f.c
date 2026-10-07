/* neighbor_mask_1040_013f: candidate for 1040:013f (115 bytes).  Verdict: RELOCATABLE.
 * symbol: neighbor_mask_1040_013f
 *
 * Segment 1040's copy of neighbor_mask: a 4-bit mask of the forest tiles
 * around map offset p (north 8, south 4, west 2, east 1) with the row
 * stride at DGROUP:0x4c40.  neighbor_is_forest is -called and defined
 * in-file as a stub.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern int g_stub_a;

extern int neighbor_is_forest_1040_00d5(int p, int tile);

int neighbor_mask_1040_013f(int p, int unused1)
{
    register int mask = 0;

    if (neighbor_is_forest_1040_00d5(p, -g_map_size.w) != 0)
        mask += 8;
    if (neighbor_is_forest_1040_00d5(p, g_map_size.w) != 0)
        mask += 4;
    if (neighbor_is_forest_1040_00d5(p, -1) != 0)
        mask += 2;
    if (neighbor_is_forest_1040_00d5(p, 1) != 0)
        mask++;
    return mask;
}
