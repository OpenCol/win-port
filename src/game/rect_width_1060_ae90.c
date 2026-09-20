/* rect_width_1060_ae90 -- shipped at 1060:ae90.
 *
 * Ported by tools/port.py from matched/game/rect_width_1060_ae90.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1060_ae90(RECT *r)
{
    return r->right - r->left;
}
