/* rect_height_1068_96c8 -- shipped at 1068:96c8.
 *
 * Ported by tools/port.py from matched/gfx/rect_height_1068_96c8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1068_96c8(RECT *r)
{
    return r->bottom - r->top;
}
