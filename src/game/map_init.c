/* map_init: candidate for 1040:4275 (64 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets the plane-2 high nibble of every map tile (DGROUP:0x4c40 by 0x4c42)
 * to -1.
 *
 * RENAMED 2026-09-27 from clear_high_nibbles: Mac map_init(void). The Mac body is the double loop over the map calling owner_set(x, y, -1), instruction for instruction the Windows loop; the 256-byte Mac size is the symbol and padding. Caller: start_new_game on both. Callee: owner_set only. Layout: after buffer_put, buffer_rect_fill, buffer_draw_box on both. Arity: void.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void owner_set(int x, int y, int v);

void map_init(void)
{
    register int x;
    register int y;

    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++)
            owner_set(x, y, -1);
    }
}
