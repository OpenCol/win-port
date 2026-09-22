/* ocean -- shipped at 1040:433e.
 *
 * Ported by tools/port.py from matched/game/ocean.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);

col_i16 ocean(col_i16 x, col_i16 y)
{
    col_i16 t;

    if (!on_map(x, y))
        return 0;
    t = map_plane0_at(x, y);
    t = (unsigned char)(t & MAP0_TERRAIN_MASK);
    return t == TERRAIN_OCEAN || t == TERRAIN_SEA_LANE;
}
