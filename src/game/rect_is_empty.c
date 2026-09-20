/* rect_is_empty -- shipped at 1038:9166.
 *
 * Ported by tools/port.py from matched/game/rect_is_empty.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1038_dfdc(void *r);
col_i16 rect_width_1038_dfbd(void *r);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 rect_is_empty(void *r)
{
    col_i16 e = 0;

    if (rect_width_1038_dfbd(r) <= 0 || rect_height_1038_dfdc(r) <= 0)
        e = 1;
    return e;
}
