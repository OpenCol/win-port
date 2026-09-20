/* rect_height_1040_aa80 -- shipped at 1040:aa80.
 *
 * Ported by tools/port.py from matched/game/rect_height_1040_aa80.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1040_aa80(RECT *r)
{
    return r->bottom - r->top;
}
