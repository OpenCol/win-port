/* rect_width_1038_8967 -- shipped at 1038:8967.
 *
 * Ported by tools/port.py from matched/game/rect_width_1038_8967.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1038_8967(RECT *r)
{
    return r->right - r->left;
}
