/* rect_height_1010_ed8b -- shipped at 1010:ed8b.
 *
 * Ported by tools/port.py from matched/game/rect_height_1010_ed8b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1010_ed8b(RECT *r)
{
    return r->bottom - r->top;
}
