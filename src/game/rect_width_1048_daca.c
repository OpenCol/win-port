/* rect_width_1048_daca -- shipped at 1048:daca.
 *
 * Ported by tools/port.py from matched/game/rect_width_1048_daca.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1048_d69b(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_width_1048_daca(void *p)
{
    return rect_width_1048_d69b(p);
}
