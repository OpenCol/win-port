/* rect_height_1078_3418 -- shipped at 1078:3418.
 *
 * Ported by tools/port.py from matched/platform/rect_height_1078_3418.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_height_1078_3418(RECT *r) { return r->bottom - r->top; }
