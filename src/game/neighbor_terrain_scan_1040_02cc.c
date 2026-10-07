/* neighbor_terrain_scan_1040_02cc: candidate for 1040:02cc (378 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Module 1040's copy of `neighbor_terrain_scan_1038_d520`: the same eight-direction walk, the
 * same five-then-three-bit mask, the same corner bookkeeping, over its OWN set of globals --
 * `g_sq_land_neighbours` at DGROUP:0x4c9e, `g_sq_land_mask` at 0x4ca1, `g_sq_corner_bits` at
 * 0x4ca2, last terrain `g_sq_terrain` at 0x4c9c with its class at 0x4c9d, and its own copy of the
 * map cursor at 0x4c82/0x4c84.
 *
 * WHAT THE THREE HOLD, since they are the reason this function exists. A neighbour counts as
 * LAND when its terrain class is neither ocean nor sea lane. `g_sq_land_mask` gets bit `i` for
 * each of the eight directions that qualifies, and `map_draw_square` matches shapes in it
 * -- `(mask & 0xdd) == 0xc1`, `(mask & 0x77) == 0x07` -- to pick a coastline tile.
 * `g_sq_corner_bits` is four bytes, one per corner: an odd direction sets bit 2 of the corner
 * it faces, an even one sets bit 4 of its own and bit 1 of the next, and the drawer turns
 * `(bits[i] << 2) + i + 0x6d` into a sprite. `g_sq_land_neighbours` is the count, and the
 * function's return value.
 *
 * FOURTEEN BYTES LONGER THAN THE TWIN, and every one of them is an addressing mode: this module
 * declares the map stride at DGROUP:0x4c40 ``, so each of the two reads costs a segment load
 * the other does not need (rule 9), and `terrain_fix_2` lives in the other module so
 * the two calls are `` rather than `PUSH CS / CALL NEAR`.
 *
 * The source is otherwise identical, `t = t & 0x1f` and the ternary included (rule 60).
 */

#include "colonization.h"

#include "terrain.h"
extern unsigned char *g_draw_map_loc;    /* DGROUP:0x4c82 */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern unsigned char g_sq_terrain;               /* DGROUP:0x4c9c */

extern char terrain_fix_2(int terrain);

int neighbor_terrain_scan_1040_02cc(void)
{
    int i;
    int j;
    unsigned char terrain;
    signed char cls;
    register int corner;
    register int next_corner;

    g_sq_land_neighbours = 0;
    g_sq_land_mask = 0;
    for (i = 0; i < 4; i++)
        g_sq_corner_bits[i] = 0;
    for (i = 0; i < 8; i++) {
        terrain = *(g_draw_map_loc + g_dir_dx[i]
              + ((int)g_dir_dy[i] < 0 ? -g_map_size.w : 0)
              + ((int)g_dir_dy[i] > 0 ? g_map_size.w : 0));
        terrain = terrain & 0x1f;
        terrain = terrain < 0x18 ? terrain & 7 : terrain;
        cls = terrain_fix_2(terrain);
        if (cls != TERRAIN_OCEAN && cls != TERRAIN_SEA_LANE) {
            g_sq_land_mask |= 1 << i;
            g_sq_land_neighbours++;
            if (i & 1) {
                corner = ((i + 1) & 7) >> 1;
                g_sq_corner_bits[corner] |= 2;
            } else {
                g_sq_terrain = terrain;
                j = i >> 1;
                corner = j;
                next_corner = (j + 1) & 3;
                g_sq_corner_bits[corner] |= 4;
                g_sq_corner_bits[next_corner] |= 1;
            }
        }
    }
    g_sq_class = terrain_fix_2(g_sq_terrain);
    return g_sq_land_neighbours;
}
