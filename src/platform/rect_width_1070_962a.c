/* rect_width_1070_962a -- shipped at 1070:962a.
 *
 * Ported by tools/port.py from matched/platform/rect_width_1070_962a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1070_962a(RECT *r)
{
    return r->right - r->left;
}
