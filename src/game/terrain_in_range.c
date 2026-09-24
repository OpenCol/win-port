/* terrain_in_range -- shipped at 1018:6361.
 *
 * Ported by tools/port.py from matched/game/terrain_in_range.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);

col_i16 terrain_in_range(col_i16 x, col_i16 y, unsigned char lo, unsigned char hi)
{
    col_i16 r = 0;
    unsigned char t;

    if (!on_map(x, y))
        goto done;
    t = map_plane0_at(x, y) & MAP0_TERRAIN_MASK;
    if (t >= lo && t <= hi)
        r = 1;
done:
    return r;
}
