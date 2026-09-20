/* rect_height_1030_e9b2 -- shipped at 1030:e9b2.
 *
 * Ported by tools/port.py from matched/game/rect_height_1030_e9b2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1030_e9b2(RECT *r)
{
    return r->bottom - r->top;
}
