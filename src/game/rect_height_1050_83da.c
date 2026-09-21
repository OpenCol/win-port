/* rect_height_1050_83da -- shipped at 1050:83da.
 *
 * Ported by tools/port.py from matched/game/rect_height_1050_83da.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1050_832b(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_1050_83da(void *p)
{
    return rect_height_1050_832b(p);
}
