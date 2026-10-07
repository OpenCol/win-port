/* tile_neighbor_clear: candidate for 1040:01b2 (282 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * For the tile above, below, left and right of the current one: set the bit when a mask
 * global is non-zero and the neighbour has none of its bits. The test is written as a
 * boolean assigned to a local, which is why each arm materialises 1 or 0 first.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern unsigned char *g_draw_site_loc;   /* DGROUP:0x4c86 */

int tile_neighbor_clear(void)
{
    int r = 0;
    int f;

    f = g_draw_mask != 0 && (*(g_draw_site_loc - g_map_size.w) & g_draw_mask) == 0;
    if (f) r += 8;
    f = g_draw_mask != 0 && (*(g_draw_site_loc + g_map_size.w) & g_draw_mask) == 0;
    if (f) r += 4;
    f = g_draw_mask != 0 && (*(g_draw_site_loc - 1) & g_draw_mask) == 0;
    if (f) r += 2;
    f = g_draw_mask != 0 && (*(g_draw_site_loc + 1) & g_draw_mask) == 0;
    if (f) r += 1;
    return r;
}
