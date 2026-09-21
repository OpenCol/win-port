/* map_plane2_set_high_nibble -- shipped at 1038:bb54.
 *
 * Ported by tools/port.py from matched/game/map_plane2_set_high_nibble.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
col_i16 on_map(col_i16 x, col_i16 y);
unsigned char * map_plane2_ptr(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */


extern col_i16 square_native_nation(col_i16 x, col_i16 y);

void map_plane2_set_high_nibble(col_i16 x, col_i16 y, col_i16 n)
{
    unsigned char *p;

    if (!on_map(x, y))
        return;
    if (!map_index_in_range(x, y))
        return;
    if (n < 4 && square_native_nation(x, y) >= 0)
        ;
    p = map_plane2_ptr(x, y);
    *p = (*p & 0xf) | (unsigned char)((n & 0xf) << 4);
}
