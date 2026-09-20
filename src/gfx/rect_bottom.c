/* rect_bottom -- shipped at 1068:986d.
 *
 * Ported by tools/port.py from matched/gfx/rect_bottom.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_bottom(RECT *r)
{
    return r->bottom;
}
