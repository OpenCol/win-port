/* neighbor_is_forest_1040_00d5: candidate for 1040:00d5 (106 bytes).  Verdict: RELOCATABLE.
 *
 * Segment 1040's copy of neighbor_is_forest: reads the terrain byte d tiles from
 * the map base (the pointer at DGROUP:0x4c82; the first argument is
 * unused): forest when the low five bits are under 24, not of the form
 * 8k+1, and above 7.
 *
 * RENAMED 2026-09-27 from tile_passable_1040_00d5: it returns 1 only for forest terrain, 8..23 (TERRAIN_FOREST_FIRST..LAST) minus the 8k+1 kinds; open land returns 0. Its only caller, neighbor_mask, builds the N/S/W/E mask used to draw the forest-edge sprite.
 */

#include "colonization.h"

extern unsigned char *g_draw_map_loc;   /* DGROUP:0x4c82 */

int neighbor_is_forest_1040_00d5(int p, int tile)
{
    register int r = 0;
    int terrain;

    terrain = g_draw_map_loc[tile];
    terrain = (unsigned char)(terrain & 0x1f);
    if (terrain >= 0x18)
        goto done;
    if ((terrain & 7) == 1)
        goto done;
    if (terrain > 7)
        r = 1;
done:
    return r;
}
