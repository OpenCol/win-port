/* main_screen_hit_region -- shipped at 1048:7653.
 *
 * Ported by tools/port.py from matched/game/main_screen_hit_region.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 empty_1040_7d3b(col_i16 x, col_i16 y, col_i16 w, col_i16 h);

col_i16 main_screen_hit_region(void)
{
    col_i16 r = 15;

    if (empty_1040_7d3b(0x131, 0x99, 0xf, 0x15))
        r = 11;
    else if (empty_1040_7d3b(0x119, 0x3f, 0x25, 0x20))
        r = 5;
    else if (empty_1040_7d3b(0, 0x198, 0x262, 0x31))
        r = 0;
    else if (empty_1040_7d3b(0x11c, 0x102, 0xa3, 0x90))
        r = 1;
    else if (empty_1040_7d3b(0x8f, 0x102, 0x8b, 0x79))
        r = 2;
    else if (empty_1040_7d3b(2, 0x102, 0x8b, 0x79))
        r = 3;
    else if (empty_1040_7d3b(0x1bf, 0x102, 0xc1, 0x93))
        r = 4;
    return r;
}
