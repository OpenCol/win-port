/* rect_width_at_8_1068_9abd -- shipped at 1068:9abd.
 *
 * Ported by tools/port.py from matched/gfx/rect_width_at_8_1068_9abd.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1068_96a9(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_width_at_8_1068_9abd(Obj *o)
{
    return rect_width_1068_96a9(((char *)((unsigned char *)(o) + 0x8)));
}
