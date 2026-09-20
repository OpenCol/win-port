/* rect_width_1010_ed6c -- shipped at 1010:ed6c.
 *
 * Ported by tools/port.py from matched/game/rect_width_1010_ed6c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1010_ed6c(RECT *r)
{
    return r->right - r->left;
}
