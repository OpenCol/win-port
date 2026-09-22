/* rect_height_at_8_1060_69bc -- shipped at 1060:69bc.
 *
 * Ported by tools/port.py from matched/game/rect_height_at_8_1060_69bc.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1060_63d4(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_at_8_1060_69bc(Obj *o)
{
    return rect_height_1060_63d4(((char *)((unsigned char *)(o) + 0x8)));
}
