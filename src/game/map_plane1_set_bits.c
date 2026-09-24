/* map_plane1_set_bits -- shipped at 1038:b87a.
 *
 * Ported by tools/port.py from matched/game/map_plane1_set_bits.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
unsigned char * map_plane1_ptr(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */


void map_plane1_set_bits(col_i16 x, col_i16 y, unsigned char mask, col_i16 on)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return;
    p = map_plane1_ptr(x, y);
    if (on)
        *p |= mask;
    else
        *p &= ~mask;
}
