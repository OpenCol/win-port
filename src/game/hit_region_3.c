/* hit_region_3 -- shipped at 1048:415a.
 *
 * Ported by tools/port.py from matched/game/hit_region_3.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hit_region_3(col_i16 x, col_i16 y)
{
    col_i16 inside = 0;
    if (x < 0x8d && x > 2 && y < 0x17b && y > 0x102)
        inside = 1;
    return inside;
}
