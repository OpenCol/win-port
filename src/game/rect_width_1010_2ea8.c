/* rect_width_1010_2ea8 -- shipped at 1010:2ea8.
 *
 * Ported by tools/port.py from matched/game/rect_width_1010_2ea8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1010_2ea8(RECT *r)
{
    return r->right - r->left;
}
