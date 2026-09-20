/* terrain_tile_index -- shipped at 1040:1408.
 *
 * Ported by tools/port.py from matched/game/terrain_tile_index.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char terrain_tile_index(col_i16 t)
{
    char r = 0;
    register unsigned char c = t & 0x1f;

    if (c <= 0x18)
        r = c % 8;
    else if (c == 0x18)
        r = 0x20;
    else if (c == 0x19)
        r = 0x24;
    else if (c == 0x1a)
        r = 0x28;
    return r;
}
