/* map_plane2_set_nibble -- shipped at 1038:ba66.
 *
 * Ported by tools/port.py from matched/game/map_plane2_set_nibble.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
unsigned char * map_plane2_ptr(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */


void map_plane2_set_nibble(col_i16 x, col_i16 y, col_i16 v)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return;
    p = map_plane2_ptr(x, y);
    *p = (*p & 0xf0) | (unsigned char)(v & 0xf);
}
