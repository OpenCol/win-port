/* square_european_nation -- shipped at 1038:be19.
 *
 * Ported by tools/port.py from matched/game/square_european_nation.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char map_plane2_high_nibble(col_i16 x, col_i16 y);
col_i16 map_index_in_range(col_i16 x, col_i16 y);

#include "terrain.h"
#include "nations.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane1_at(col_i16 x, col_i16 y);

col_i16 square_european_nation(col_i16 x, col_i16 y)
{
    col_i16 n = NATION_NONE;

    if (!map_index_in_range(x, y))
        return NATION_NONE;
    if (on_map(x, y) && (map_plane1_at(x, y) & MAP1_SETTLEMENT)) {
        n = map_plane2_high_nibble(x, y);
        if (n >= NATION_EUROPEAN_COUNT)
            n = NATION_NONE;
    }
    return n;
}
