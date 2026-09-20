/* rect_width_1068_d79c -- shipped at 1068:d79c.
 *
 * Ported by tools/port.py from matched/gfx/rect_width_1068_d79c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1068_d79c(RECT *r)
{
    return r->right - r->left;
}
