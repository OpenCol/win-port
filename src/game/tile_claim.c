/* tile_claim -- shipped at 1038:2a2a.
 *
 * Ported by tools/port.py from matched/game/tile_claim.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 find_adjacent_owner(col_i16 x, col_i16 y, col_i16 n);

extern col_i16 square_nation(col_i16 x, col_i16 y);



col_i16 tile_claim(col_i16 x, col_i16 y, col_i16 n)
{
    col_i16 r = 0;

    if (square_nation(x, y) < 0)
        r = find_adjacent_owner(x, y, n);
    return r;
}
