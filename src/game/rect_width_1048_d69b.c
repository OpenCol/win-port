/* rect_width_1048_d69b -- shipped at 1048:d69b.
 *
 * Ported by tools/port.py from matched/game/rect_width_1048_d69b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1048_d69b(RECT *r)
{
    return r->right - r->left;
}
