/* rect_width_1038_dfbd -- shipped at 1038:dfbd.
 *
 * Ported by tools/port.py from matched/game/rect_width_1038_dfbd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1038_dfbd(RECT *r)
{
    return r->right - r->left;
}
