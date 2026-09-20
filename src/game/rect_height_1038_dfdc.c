/* rect_height_1038_dfdc -- shipped at 1038:dfdc.
 *
 * Ported by tools/port.py from matched/game/rect_height_1038_dfdc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1038_dfdc(RECT *r)
{
    return r->bottom - r->top;
}
