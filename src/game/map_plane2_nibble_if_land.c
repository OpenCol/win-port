/* map_plane2_nibble_if_land -- shipped at 1038:bbda.
 *
 * Ported by tools/port.py from matched/game/map_plane2_nibble_if_land.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
col_i16 on_map(col_i16 x, col_i16 y);
unsigned char map_plane2_nibble(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 ocean(col_i16 x, col_i16 y);

col_i16 map_plane2_nibble_if_land(col_i16 x, col_i16 y)
{
    col_i16 n = -1;

    if (!map_index_in_range(x, y))
        return -1;
    if (on_map(x, y) && !ocean(x, y))
        n = map_plane2_nibble(x, y);
    return n;
}
