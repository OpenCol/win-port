/* rect_height_1020_ad08 -- shipped at 1020:ad08.
 *
 * Ported by tools/port.py from matched/game/rect_height_1020_ad08.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1020_b03e(RECT *r);

/* same object: 4-byte `push cs; call ` */


col_i16 rect_height_1020_ad08(RECT *p)
{
    return rect_height_1020_b03e(p);
}
