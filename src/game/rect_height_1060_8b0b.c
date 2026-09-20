/* rect_height_1060_8b0b -- shipped at 1060:8b0b.
 *
 * Ported by tools/port.py from matched/game/rect_height_1060_8b0b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1060_8b0b(RECT *r)
{
    return r->bottom - r->top;
}
