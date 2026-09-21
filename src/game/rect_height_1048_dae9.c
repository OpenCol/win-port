/* rect_height_1048_dae9 -- shipped at 1048:dae9.
 *
 * Ported by tools/port.py from matched/game/rect_height_1048_dae9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1048_d6ba(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_1048_dae9(void *p)
{
    return rect_height_1048_d6ba(p);
}
