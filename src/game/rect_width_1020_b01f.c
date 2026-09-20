/* rect_width_1020_b01f -- shipped at 1020:b01f.
 *
 * Ported by tools/port.py from matched/game/rect_width_1020_b01f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1020_b01f(RECT *r)
{
    return r->right - r->left;
}
