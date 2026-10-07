/* tile_neighbor_match: candidate for 1038:d386 (201 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The sibling of tile_neighbor_mask (1040:0446) in the game segment: for the tile above,
 * below, left and right of the current one, tests whether its byte masked with 0xa0
 * EQUALS the argument, and packs the four results into bits 3..0. The 0xa0 is held in a
 * local, and here the map width and tile pointer are globals.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */
extern unsigned char *g_tile_map_loc;   /* DGROUP:0x4c72 */

int tile_neighbor_match(int value, int unused1)
{
    int r = 0;
    int mask = 0xa0;

    if ((*(g_tile_map_loc - g_map_size.w) & mask) == value) r += 8;
    if ((*(g_tile_map_loc + g_map_size.w) & mask) == value) r += 4;
    if ((*(g_tile_map_loc - 1) & mask) == value) r += 2;
    if ((*(g_tile_map_loc + 1) & mask) == value) r += 1;
    return r;
}
