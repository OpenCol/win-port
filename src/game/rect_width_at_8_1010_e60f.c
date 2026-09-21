/* rect_width_at_8_1010_e60f -- shipped at 1010:e60f.
 *
 * Ported by tools/port.py from matched/game/rect_width_at_8_1010_e60f.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1010_ed6c(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_width_at_8_1010_e60f(Obj *o)
{
    return rect_width_1010_ed6c(((char *)((unsigned char *)(o) + 0x8)));
}
