/* rect_width_1050_830c -- shipped at 1050:830c.
 *
 * Ported by tools/port.py from matched/game/rect_width_1050_830c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1050_830c(RECT *r)
{
    return r->right - r->left;
}
