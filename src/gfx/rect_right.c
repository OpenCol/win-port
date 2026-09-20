/* rect_right -- shipped at 1068:9886.
 *
 * Ported by tools/port.py from matched/gfx/rect_right.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_right(RECT *r)
{
    return r->right;
}
