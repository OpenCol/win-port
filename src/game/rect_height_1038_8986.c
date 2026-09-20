/* rect_height_1038_8986 -- shipped at 1038:8986.
 *
 * Ported by tools/port.py from matched/game/rect_height_1038_8986.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1038_8986(RECT *r)
{
    return r->bottom - r->top;
}
