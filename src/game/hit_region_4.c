/* hit_region_4 -- shipped at 1048:41c7.
 *
 * Ported by tools/port.py from matched/game/hit_region_4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hit_region_4(col_i16 x, col_i16 y)
{
    col_i16 inside = 0;
    if (x < 0x280 && x > 0x1bf && y < 0x195 && y > 0x102)
        inside = 1;
    return inside;
}
