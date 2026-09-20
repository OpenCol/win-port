/* rect_height_1058_bd4c -- shipped at 1058:bd4c.
 *
 * Ported by tools/port.py from matched/game/rect_height_1058_bd4c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1058_bd4c(RECT *r)
{
    return r->bottom - r->top;
}
