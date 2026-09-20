/* rect_width_1040_a903 -- shipped at 1040:a903.
 *
 * Ported by tools/port.py from matched/game/rect_width_1040_a903.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1040_a903(RECT *r)
{
    return r->right - r->left;
}
