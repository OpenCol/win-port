/* rect_height_1020_b03e -- shipped at 1020:b03e.
 *
 * Ported by tools/port.py from matched/game/rect_height_1020_b03e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1020_b03e(RECT *r)
{
    return r->bottom - r->top;
}
