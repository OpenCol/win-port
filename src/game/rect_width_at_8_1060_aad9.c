/* rect_width_at_8_1060_aad9 -- shipped at 1060:aad9.
 *
 * Ported by tools/port.py from matched/game/rect_width_at_8_1060_aad9.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1060_ae90(RECT *r);

/* same object: 4-byte `push cs; call ` */

col_i16 rect_width_at_8_1060_aad9(Obj *o)
{
    return rect_width_1060_ae90(((char *)((unsigned char *)(o) + 0x8)));
}
