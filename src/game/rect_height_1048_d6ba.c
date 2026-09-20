/* rect_height_1048_d6ba -- shipped at 1048:d6ba.
 *
 * Ported by tools/port.py from matched/game/rect_height_1048_d6ba.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1048_d6ba(RECT *r)
{
    return r->bottom - r->top;
}
