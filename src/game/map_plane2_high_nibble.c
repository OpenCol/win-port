/* map_plane2_high_nibble -- shipped at 1038:bb13.
 *
 * Ported by tools/port.py from matched/game/map_plane2_high_nibble.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char map_plane2_at(col_i16 x, col_i16 y);

#include "terrain.h"
#include "nations.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

char map_plane2_high_nibble(col_i16 x, col_i16 y)
{
    col_i16 v = (map_plane2_at(x, y) & 0xf0) >> 4;

    if (v == MAP2_NIBBLE_NONE)
        v = NATION_NONE;
    return v;
}
