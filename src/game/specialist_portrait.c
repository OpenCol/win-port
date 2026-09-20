/* specialist_portrait -- shipped at 1040:1d76.
 *
 * Ported by tools/port.py from matched/game/specialist_portrait.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 specialist_portrait(col_i16 spec)
{
    col_i16 r;

    switch (spec) {
    case 0x14:
        r = 0x3b;
        break;
    case 0x15:
        r = 0x3c;
        break;
    case 0x16:
        r = 0x3d;
        break;
    case 0x18:
        r = 0x3e;
        break;
    case 0x19:
        r = 0x6b;
        break;
    case 0x1a:
        r = 0x6c;
        break;
    case 0x1b:
        r = 0x43;
        break;
    case 0x13:
    case 0x1c:
        r = 0x65;
        break;
    default:
        r = spec + 0x52;
        break;
    }
    return r;
}
