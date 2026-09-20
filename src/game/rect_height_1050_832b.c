/* rect_height_1050_832b -- shipped at 1050:832b.
 *
 * Ported by tools/port.py from matched/game/rect_height_1050_832b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1050_832b(RECT *r)
{
    return r->bottom - r->top;
}
