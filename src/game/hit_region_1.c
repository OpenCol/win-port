/* hit_region_1 -- shipped at 1048:4190.
 *
 * Ported by tools/port.py from matched/game/hit_region_1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hit_region_1(col_i16 x, col_i16 y)
{
    col_i16 inside = 0;
    if (x < 0x1bf && x > 0x11c && y < 0x192 && y > 0x102)
        inside = 1;
    return inside;
}
