/* rect_width_1058_bd2d -- shipped at 1058:bd2d.
 *
 * Ported by tools/port.py from matched/game/rect_width_1058_bd2d.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1058_bd2d(RECT *r)
{
    return r->right - r->left;
}
