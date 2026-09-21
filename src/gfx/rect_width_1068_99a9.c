/* rect_width_1068_99a9 -- shipped at 1068:99a9.
 *
 * Ported by tools/port.py from matched/gfx/rect_width_1068_99a9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1068_96a9(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_width_1068_99a9(void *p)
{
    return rect_width_1068_96a9(p);
}
