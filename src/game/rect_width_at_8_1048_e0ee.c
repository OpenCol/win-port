/* rect_width_at_8_1048_e0ee -- shipped at 1048:e0ee.
 *
 * Ported by tools/port.py from matched/game/rect_width_at_8_1048_e0ee.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1048_d69b(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_width_at_8_1048_e0ee(Obj *o)
{
    return rect_width_1048_d69b(((char *)((unsigned char *)(o) + 0x8)));
}
