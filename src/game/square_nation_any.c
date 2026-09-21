/* square_nation_any -- shipped at 1038:bf7c.
 *
 * Ported by tools/port.py from matched/game/square_nation_any.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
col_i16 square_nation(col_i16 x, col_i16 y);
col_i16 square_nation_bit0(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */



col_i16 square_nation_any(col_i16 x, col_i16 y)
{
    col_i16 n;

    if (!map_index_in_range(x, y))
        return -1;
    n = square_nation(x, y);
    if (n < 0)
        n = square_nation_bit0(x, y);
    return n;
}
