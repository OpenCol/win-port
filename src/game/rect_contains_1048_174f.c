/* rect_contains_1048_174f -- shipped at 1048:174f.
 *
 * Ported by tools/port.py from matched/game/rect_contains_1048_174f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_contains_1048_174f(RECT *r, col_i16 x, col_i16 y)
{
    if (r->left <= x && r->right >= x) {
        if (r->top <= y && r->bottom >= y)
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}
