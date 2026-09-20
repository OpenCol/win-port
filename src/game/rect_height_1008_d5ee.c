/* rect_height_1008_d5ee -- shipped at 1008:d5ee.
 *
 * Ported by tools/port.py from matched/game/rect_height_1008_d5ee.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1008_d5ee(RECT *r)
{
    return r->bottom - r->top;
}
