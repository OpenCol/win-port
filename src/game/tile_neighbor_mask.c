/* tile_neighbor_mask: candidate for 1040:0446 (187 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Tests `mask` against the tile above, below, left and right of the current map tile
 * (a pointer into the map, stepping by the map width) and packs the four results
 * into bits 3..0. The map width is a global in the data segment; the tile pointer is
 * a global.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern unsigned char *g_draw_map_loc;   /* DGROUP:0x4c82 */

int tile_neighbor_mask(int mask, int unused1)
{
    int r = 0;

    if (*(g_draw_map_loc - g_map_size.w) & mask) r += 8;
    if (*(g_draw_map_loc + g_map_size.w) & mask) r += 4;
    if (*(g_draw_map_loc - 1) & mask) r += 2;
    if (*(g_draw_map_loc + 1) & mask) r += 1;
    return r;
}
