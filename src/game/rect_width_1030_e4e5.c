/* rect_width_1030_e4e5 -- shipped at 1030:e4e5.
 *
 * Ported by tools/port.py from matched/game/rect_width_1030_e4e5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1030_e4e5(RECT *r)
{
    return r->right - r->left;
}
