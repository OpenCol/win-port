/* ocean_tile_yield -- shipped at 1028:b173.
 *
 * Ported by tools/port.py from matched/game/ocean_tile_yield.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);

col_i16 ocean_tile_yield(col_i16 x, col_i16 y)
{
    register col_i16 n;
    register col_i16 t;

    n = 0;
    t = terrain_at(x, y);
    if (t != TERRAIN_OCEAN) {
        if (t != TERRAIN_SEA_LANE)
            goto out;
    }
    n++;
    if (t == TERRAIN_SEA_LANE)
        goto out;
    n++;
    if (tile_decoration(x, y) == -1)
        goto out;
    n += 3;
out:
    return n;
}
