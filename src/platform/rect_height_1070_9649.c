/* rect_height_1070_9649 -- shipped at 1070:9649.
 *
 * Ported by tools/port.py from matched/platform/rect_height_1070_9649.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1070_9649(RECT *r)
{
    return r->bottom - r->top;
}
