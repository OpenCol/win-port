/* foreign_nation_at -- shipped at 1038:bfdd.
 *
 * Ported by tools/port.py from matched/game/foreign_nation_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_pair[][0x13c];     SEG20:0xc328
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char map_plane2_high_nibble(col_i16 x, col_i16 y);
col_i16 map_index_in_range(col_i16 x, col_i16 y);

#include "terrain.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char map_plane1_at(col_i16 x, col_i16 y);

col_i16 foreign_nation_at(col_i16 x, col_i16 y, col_i16 me)
{
    col_i16 r = -1;
    col_i16 n;

    if (!map_index_in_range(x, y))
        return -1;
    if (map_plane1_at(x, y) & (MAP1_ROAD | MAP1_PLOWED)) {
        n = map_plane2_high_nibble(x, y);
        if (n >= 0 && n < 4 && n != me && (g_nation_pair[me][n] & 0x40))
            r = n;
    }
    return r;
}
