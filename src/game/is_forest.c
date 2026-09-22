/* is_forest -- shipped at 1040:43aa.
 *
 * Ported by tools/port.py from matched/game/is_forest.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"

extern unsigned char map_plane0_at(col_i16 x, col_i16 y);

col_i16 is_forest(col_i16 x, col_i16 y)
{
    col_i16 t = map_plane0_at(x, y);

    t = (unsigned char)(t & MAP0_TERRAIN_MASK);
    return (t >= TERRAIN_BOREAL_FOREST && t < TERRAIN_BOREAL_FOREST_2)
        || (t >= TERRAIN_BOREAL_FOREST_2 && t < TERRAIN_ARCTIC);
}
