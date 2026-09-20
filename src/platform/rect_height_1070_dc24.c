/* rect_height_1070_dc24 -- shipped at 1070:dc24.
 *
 * Ported by tools/port.py from matched/platform/rect_height_1070_dc24.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1070_dc24(RECT *r)
{
    return r->bottom - r->top;
}
