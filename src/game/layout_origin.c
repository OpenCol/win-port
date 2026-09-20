/* layout_origin -- shipped at 1048:1059.
 *
 * Ported by tools/port.py from matched/game/layout_origin.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void layout_origin(void *unused, col_i16 sel, col_i16 *a, col_i16 *b)
{
    switch (sel) {
    case 0:
        *a = 0x95;
        *b = 0x22;
        break;
    case 1:
        *a = 0x95;
        *b = 0x22;
        break;
    case 2:
        *a = 0xc;
        *b = 0x22;
        break;
    case 3:
    default:
        *a = 0x44;
        *b = 0x22;
        break;
    }
}
