/* rect_height_1060_aeaf -- shipped at 1060:aeaf.
 *
 * Ported by tools/port.py from matched/game/rect_height_1060_aeaf.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1060_aeaf(RECT *r)
{
    return r->bottom - r->top;
}
