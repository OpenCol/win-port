/* rect_height_1010_2ec7 -- shipped at 1010:2ec7.
 *
 * Ported by tools/port.py from matched/game/rect_height_1010_2ec7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1010_2ec7(RECT *r)
{
    return r->bottom - r->top;
}
