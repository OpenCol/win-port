/* ScanOceanDither: candidate for 1008:7cb5 (376 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 376, not the census 367. The trailing 9 bytes are the dead second epilogue.
 *
 * THE COASTLINE OF A WATER SQUARE. `ScanOcean` points DGROUP:0x4b94 at a square's plane-0
 * byte and calls this only when the square is water (terrain class 0x19 or 0x1a). The eight
 * neighbours are read through that pointer -- the direction's dx, plus or minus one map row for
 * its dy -- and every one that is not water counts: the count is returned, and each lights
 * flags in the four corner bytes at DGROUP:0x4b90. A diagonal neighbour sets bit 1 of the
 * corner it touches; an orthogonal one sets bit 2 of the corner before it and bit 0 of the one
 * after, and its terrain is kept in 0x4b8f -- the last one wins -- with its class left in 0x4b8e
 * at the end.
 *
 * A neighbour's terrain is its low five bits, and below 0x18 only the low three: the rest are
 * the overlay bits.
 *
 * THE POINTER IS ``: every addition is normalized through `__AHSHIFT` -- `ADD AX / ADC DX,0
 * / SHL DX,CL / ADD DX` -- and the neighbour address is one expression added left to right, the
 * intermediate pointers pushed between terms. `(int)g_dir_dy[d] < 0` loads and `CBW`s where
 * the bare char compare folds to `CMP BYTE` (rule 51); `mask |= (unsigned char)(1 << d)` shifts
 * in AL (rule 58); `t = t & 0x1f` and the ternary store once each (rule 60). The corner indices
 * go through the two `register` locals, SI and DI.
 *
 * RENAMED 2026-09-27 from coast_neighbours: Mac ScanOceanDither(void), 342 bytes, against 376. Caller: ScanOcean on Mac, cell_draw_terrain (= ScanOcean, below) here. Callee: terrain_fix_2 x2 on both. Layout: the Mac CODE 26 run clean_up, ScanOceanDither, ScanOcean, SetupDitherMap, setup_building is Windows 1008 clean_up, 7cb5, 7e2d, 7ec9, setup_building, one for one between two bound neighbours. Arity: void on both. The body counts a water square's land neighbours, the dither of the coastline.
 */

#include "colonization.h"

#include "terrain.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern char terrain_fix_2(int terrain);

int ScanOceanDither(void)
{
    int dir;
    int h;
    unsigned char neighbour_terrain;
    signed char cls;
    int count;
    int mask;
    register int corner;
    register int next_corner;

    count = 0;
    mask = 0;
    for (dir = 0; dir < 4; dir++)
        g_coast_edges[dir] = 0;
    for (dir = 0; dir < 8; dir++) {
        neighbour_terrain = *(g_cur_cell + g_dir_dx[dir] + ((int)g_dir_dy[dir] < 0 ? -g_map_size.w : 0)
                                       + ((int)g_dir_dy[dir] > 0 ? g_map_size.w : 0));
        neighbour_terrain = neighbour_terrain & 0x1f;
        neighbour_terrain = neighbour_terrain < 0x18 ? neighbour_terrain & 7 : neighbour_terrain;
        cls = terrain_fix_2(neighbour_terrain);
        if (cls != TERRAIN_OCEAN && cls != TERRAIN_SEA_LANE) {
            mask |= (unsigned char)(1 << dir);
            count++;
            if (dir & 1) {
                corner = ((dir + 1) & 7) >> 1;
                g_coast_edges[corner] |= 2;
            } else {
                g_cell_terrain = neighbour_terrain;
                h = dir >> 1;
                corner = h;
                next_corner = (h + 1) & 3;
                g_coast_edges[corner] |= 4;
                g_coast_edges[next_corner] |= 1;
            }
        }
    }
    g_cell_class = terrain_fix_2(g_cell_terrain);
    return count;
}
