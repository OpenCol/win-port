/* hittest_region -- shipped at 1030:0ec7.
 *
 * Ported by tools/port.py from matched/game/hittest_region.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hittest_region(void *unused, col_i16 x, col_i16 y)
{
    col_i16 r = 0;

    if (y > 0x11 && y < 0xe7) {
        if (x > 0xff && x < 0x188) r = 1;
        else if (x > 0x1d1 && x < 0x25a) r = 2;
    } else if (y > 0xf8 && y < 0x1cf) {
        if (x > 0x2d && x < 0xb6) r = 3;
        else if (x > 0xff && x < 0x188) r = 4;
        else if (x > 0x1d1 && x < 0x25a) r = 5;
    }
    return r;
}
