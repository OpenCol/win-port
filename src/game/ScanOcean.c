/* ScanOcean: candidate for 1008:7e2d (156 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The terrain a map square is drawn as. It points DGROUP:0x4b94 at the square's plane-0 byte,
 * keeps the raw byte in 0x4b8f and its class in 0x4b8e. A water square (class 0x19 or 0x1a)
 * counts its land neighbours with ScanOceanDither, and water with none gives -1;
 * anything else is the class, masked to its low three bits below 0x18.
 *
 * `is_water` is never initialised: for a land square the test reads whatever the stack held, and
 * only a zero there sends it on to the class. `save` is written and never read. The trailing
 * nine bytes are the dead second epilogue of the explicit `return` (rule 44).
 *
 * RENAMED 2026-09-27 from cell_draw_terrain: Mac ScanOcean(short, short), 172 bytes, against 156. Caller: SetupDitherMap (7ec9, below) on both. Callees: map_loc, terrain_fix_2 and ScanOceanDither on both; Mac seq map_loc, terrain_fix_2, ScanOceanDither is the Windows order. Layout: as 7cb5. Arity: 2 (int x, int y).
 */

#include "colonization.h"

#include "terrain.h"

extern unsigned char * map_loc(int x, int y);
extern char terrain_fix_2(int terrain);

/* same object: 4-byte `push cs; call ` */
extern int ScanOceanDither(void);

char ScanOcean(int x, int y)
{
    char is_water;
    char save;
    int r;
    register int n;

    n = 0;
    r = -1;
    g_cur_cell = map_loc(x, y);
    g_cell_terrain = *g_cur_cell;
    g_cell_class = terrain_fix_2(g_cell_terrain);
    if (g_cell_class == TERRAIN_OCEAN || g_cell_class == TERRAIN_SEA_LANE) {
        save = g_cell_class;
        n = ScanOceanDither();
        is_water = 1;
    }
    if (is_water == 0 || n != 0) {
        if (g_cell_class < TERRAIN_ARCTIC)
            r = g_cell_class & 7;
        else
            r = g_cell_class;
    }
    return r;
}
