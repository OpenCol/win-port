/* rect_width_1078_33f9 -- shipped at 1078:33f9.
 *
 * Ported by tools/port.py from matched/platform/rect_width_1078_33f9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect_width_1078_33f9(RECT *r) { return r->right - r->left; }
