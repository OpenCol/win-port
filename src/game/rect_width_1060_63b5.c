/* rect_width_1060_63b5 -- shipped at 1060:63b5.
 *
 * Ported by tools/port.py from matched/game/rect_width_1060_63b5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1060_63b5(RECT *r)
{
    return r->right - r->left;
}
