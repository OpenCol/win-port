/* rect_height_at_8_1068_9ae0 -- shipped at 1068:9ae0.
 *
 * Ported by tools/port.py from matched/gfx/rect_height_at_8_1068_9ae0.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1068_96c8(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_at_8_1068_9ae0(Obj *o)
{
    return rect_height_1068_96c8(((char *)((unsigned char *)(o) + 0x8)));
}
