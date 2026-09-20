/* get_w_2_1050_8b8a -- shipped at 1050:8b8a.
 *
 * Ported by tools/port.py from matched/game/get_w_2_1050_8b8a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1050_922e(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 get_w_2_1050_8b8a(void *p)
{
    return get_w_2_1050_922e(p);
}
