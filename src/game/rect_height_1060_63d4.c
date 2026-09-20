/* rect_height_1060_63d4 -- shipped at 1060:63d4.
 *
 * Ported by tools/port.py from matched/game/rect_height_1060_63d4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1060_63d4(RECT *r)
{
    return r->bottom - r->top;
}
