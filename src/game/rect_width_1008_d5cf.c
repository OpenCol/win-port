/* rect_width_1008_d5cf -- shipped at 1008:d5cf.
 *
 * Ported by tools/port.py from matched/game/rect_width_1008_d5cf.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1008_d5cf(RECT *r)
{
    return r->right - r->left;
}
