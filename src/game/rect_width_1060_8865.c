/* rect_width_1060_8865 -- shipped at 1060:8865.
 *
 * Ported by tools/port.py from matched/game/rect_width_1060_8865.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1060_8865(RECT *r)
{
    return r->right - r->left;
}
