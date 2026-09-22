/* rect_height_at_8_1010_e681 -- shipped at 1010:e681.
 *
 * Ported by tools/port.py from matched/game/rect_height_at_8_1010_e681.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1010_ed8b(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_height_at_8_1010_e681(Obj *o)
{
    return rect_height_1010_ed8b(((char *)((unsigned char *)(o) + 0x8)));
}
