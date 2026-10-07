/* tile_neighbor_match_alt: candidate for 1040:0000 (213 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The same routine as tile_neighbor_match (1038:d386), in the segment where the map width
 * is reached as a global, like tile_neighbor_mask (1040:0446) beside it: for the tile above,
 * below, left and right of the current one, tests whether its byte masked with 0xa0
 * EQUALS the argument, and packs the four results into bits 3..0. The 0xa0 is held in a
 * local, and here the tile pointer is a global while the map width is a one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern unsigned char *g_draw_map_loc;   /* DGROUP:0x4c82 */

int tile_neighbor_match_alt(int value, int unused1)
{
    int r = 0;
    int mask = 0xa0;

    if ((*(g_draw_map_loc - g_map_size.w) & mask) == value) r += 8;
    if ((*(g_draw_map_loc + g_map_size.w) & mask) == value) r += 4;
    if ((*(g_draw_map_loc - 1) & mask) == value) r += 2;
    if ((*(g_draw_map_loc + 1) & mask) == value) r += 1;
    return r;
}
