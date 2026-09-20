/* hittest_panel -- shipped at 1010:4538.
 *
 * Ported by tools/port.py from matched/game/hittest_panel.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 hittest_panel(col_i16 x, col_i16 y)
{
    col_i16 r = 0x14;

    if (x > 0 && x < 0x190 && y > 0 && y < 0x109)
        r = 2;
    else if (x > 0 && x < 0x262 && y > 0x186 && y < 0x1b8)
        r = 5;
    else if (x > 0x19e && x < 0x258 && y > 0x110 && y < 0x183)
        r = 4;
    else if (x > 0x1a8 && x < 0x268 && y > 0x24 && y < 0xe4)
        r = 1;
    else if (x > 0 && x < 0xf0 && y > 0x110 && y < 0x183)
        r = 0;
    else if (x > 0xf2 && x < 0x19c && y > 0x110 && y < 0x184) {
        if (x > 0xfa && x < 0x194 && y > 0x161 && y < 0x184)
            r = 7;
        else
            r = 8;
    }
    return r;
}
