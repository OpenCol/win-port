/* rect_height_1068_d7bb -- shipped at 1068:d7bb.
 *
 * Ported by tools/port.py from matched/gfx/rect_height_1068_d7bb.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1068_d7bb(RECT *r)
{
    return r->bottom - r->top;
}
