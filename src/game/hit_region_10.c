/* hit_region_10 -- shipped at 1048:41fe.
 *
 * Ported by tools/port.py from matched/game/hit_region_10.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hit_region_10(col_i16 x, col_i16 y)
{
    col_i16 inside = 0;
    if (x < 0x1b0 && x > 0x122 && y < 0x190 && y > 0x16e)
        inside = 1;
    return inside;
}
