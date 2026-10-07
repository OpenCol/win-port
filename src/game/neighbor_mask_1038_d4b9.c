/* neighbor_mask_1038_d4b9: candidate for 1038:d4b9 (103 bytes).  Verdict: RELOCATABLE.
 * symbol: neighbor_mask_1038_d4b9
 *
 * A 4-bit mask of the forest tiles around map offset p: north (8), south
 * (4), west (2), east (1), using the row stride at DGROUP:0x4c40.
 * neighbor_is_forest is -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */
extern int g_stub_a;

extern int neighbor_is_forest_1038_d44f(int p, int tile);

int neighbor_mask_1038_d4b9(int p, int unused1)
{
    register int mask = 0;

    if (neighbor_is_forest_1038_d44f(p, -g_map_size.w) != 0)
        mask += 8;
    if (neighbor_is_forest_1038_d44f(p, g_map_size.w) != 0)
        mask += 4;
    if (neighbor_is_forest_1038_d44f(p, -1) != 0)
        mask += 2;
    if (neighbor_is_forest_1038_d44f(p, 1) != 0)
        mask++;
    return mask;
}
