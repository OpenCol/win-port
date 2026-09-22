/* rect_height_at_8_1038_8752 -- shipped at 1038:8752.
 *
 * Ported by tools/port.py from matched/game/rect_height_at_8_1038_8752.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1038_8986(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_at_8_1038_8752(Obj *o)
{
    return rect_height_1038_8986(((char *)((unsigned char *)(o) + 0x8)));
}
