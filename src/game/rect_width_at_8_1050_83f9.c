/* rect_width_at_8_1050_83f9 -- shipped at 1050:83f9.
 *
 * Ported by tools/port.py from matched/game/rect_width_at_8_1050_83f9.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1050_830c(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_width_at_8_1050_83f9(Obj *o)
{
    return rect_width_1050_830c(((char *)((unsigned char *)(o) + 0x8)));
}
