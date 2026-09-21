/* map_plane2_nibble -- shipped at 1038:ba45.
 *
 * Ported by tools/port.py from matched/game/map_plane2_nibble.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_i;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char map_plane2_at(col_i16 x, col_i16 y);

unsigned char map_plane2_nibble(col_i16 x, col_i16 y)
{
    return map_plane2_at(x, y) & 0xf;
}
