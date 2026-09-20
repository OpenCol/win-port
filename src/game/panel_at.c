/* panel_at -- shipped at 1030:05c7.
 *
 * Ported by tools/port.py from matched/game/panel_at.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 panel_at(void *unused, col_i16 x, col_i16 y)
{
    col_i16 r = 0;
    if (x >= 0xdf && x <= 0x18f) {
        if (y >= 0x1e && y <= 0xe6)
            r = 1;
        else if (y >= 0xfa && y <= 0x1c2)
            r = 3;
    } else if (x >= 0x1a6 && x <= 0x256) {
        if (y >= 0x1e && y <= 0xe6)
            r = 2;
        else if (y >= 0xfa && y <= 0x1c2)
            r = 4;
    }
    return r;
}
