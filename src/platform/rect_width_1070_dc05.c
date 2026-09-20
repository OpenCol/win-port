/* rect_width_1070_dc05 -- shipped at 1070:dc05.
 *
 * Ported by tools/port.py from matched/platform/rect_width_1070_dc05.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1070_dc05(RECT *r)
{
    return r->right - r->left;
}
