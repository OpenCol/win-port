/* rect_width_1010_2b62 -- shipped at 1010:2b62.
 *
 * Ported by tools/port.py from matched/game/rect_width_1010_2b62.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1010_2ea8(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_width_1010_2b62(void *p)
{
    return rect_width_1010_2ea8(p);
}
