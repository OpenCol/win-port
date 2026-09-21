/* rect_height_at_8_1060_8842 -- shipped at 1060:8842.
 *
 * Ported by tools/port.py from matched/game/rect_height_at_8_1060_8842.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1060_8b0b(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_height_at_8_1060_8842(Obj *o)
{
    return rect_height_1060_8b0b(((char *)((unsigned char *)(o) + 0x8)));
}
