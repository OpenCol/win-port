/* neighbor_terrain_scan_1038_d520: candidate for 1038:d520 (364 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Walks the eight tiles around the current map position and leaves four things behind: a count
 * `g_tile_land_neighbours` at DGROUP:0x4c68, `g_tile_land_mask` at 0x4c6b,
 * `g_tile_corner_bits` (four bytes) at 0x4c6e, and
 * the terrain of the last ORTHOGONAL neighbour at 0x4c6d with its class at 0x4c6c.
 *
 * The eight directions come from the `signed char` tables at SEG27:0 and SEG27:9, and the
 * address is ONE `` EXPRESSION: the base plus dx, plus a row up, plus a row down, with the
 * two rows chosen by two ternaries. Borland pushes the partial sum across each one and pops it
 * back, which is why `PUSH AX / PUSH DX ... POP BX / POP CX` appears twice in the middle of a
 * pointer computation.
 *
 * The tile byte is masked to five bits and then to three unless it is 0x18 or above, so the
 * high terrain codes keep two more bits than the low ones. Classes 0x19 and 0x1a are the two
 * that count as absent; everything else sets its direction bit and bumps the count.
 *
 * ODD DIRECTIONS ARE DIAGONALS. They mark bit 1 of one corner; even ones mark bit 2 of a corner
 * and bit 0 of the next round the ring, and only they update the remembered terrain -- so the
 * corner bytes carry a three-bit summary of which of the two edges and the diagonal at each
 * corner is land.
 *
 * TWO SPELLINGS THE BYTES FIXED, both about where the work happens. `t = t & 0x1f` loads,
 * masks and stores where `t &= 0x1f` is one in-place `AND BYTE` (rule 60), and
 * `t = t < 0x18 ? t & 7 : t` computes both arms into AL and stores once where the `if` form
 * stores conditionally. Neither is visible anywhere but in the instruction count.
 *
 * The direction tests carry rule 51's written-out `(int)` cast: `CBW` then `OR AX,AX`, where
 * the plain comparison is one `CMP BYTE`.
 *
 * See `neighbor_terrain_scan_1040_02cc` for what the three hold; module 1040's copy carries
 * the reading, and this is the same body over this module's own globals.
 */

#include "colonization.h"

#include "terrain.h"
extern unsigned char *g_tile_map_loc;    /* DGROUP:0x4c72 */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

/* same object: 4-byte `push cs; call ` */
extern char terrain_fix_2(int terrain);

int neighbor_terrain_scan_1038_d520(void)
{
    int i;
    int j;
    unsigned char terrain;
    signed char cls;
    register int corner;
    register int next_corner;

    g_tile_land_neighbours = 0;
    g_tile_land_mask = 0;
    for (i = 0; i < 4; i++)
        g_tile_corner_bits[i] = 0;
    for (i = 0; i < 8; i++) {
        terrain = *(g_tile_map_loc + g_dir_dx[i]
              + ((int)g_dir_dy[i] < 0 ? -g_map_size.w : 0)
              + ((int)g_dir_dy[i] > 0 ? g_map_size.w : 0));
        terrain = terrain & 0x1f;
        terrain = terrain < 0x18 ? terrain & 7 : terrain;
        cls = terrain_fix_2(terrain);
        if (cls != TERRAIN_OCEAN && cls != TERRAIN_SEA_LANE) {
            g_tile_land_mask |= 1 << i;
            g_tile_land_neighbours++;
            if (i & 1) {
                corner = ((i + 1) & 7) >> 1;
                g_tile_corner_bits[corner] |= 2;
            } else {
                g_tile_terrain = terrain;
                j = i >> 1;
                corner = j;
                next_corner = (j + 1) & 3;
                g_tile_corner_bits[corner] |= 4;
                g_tile_corner_bits[next_corner] |= 1;
            }
        }
    }
    g_tile_terrain_class = terrain_fix_2(g_tile_terrain);
    return g_tile_land_neighbours;
}
