/* call_ed8b_at_8 -- shipped at 1010:e681.
 *
 * Ported by tools/port.py from matched/game/call_ed8b_at_8.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1010_ed8b(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_ed8b_at_8(Obj *o)
{
    return rect_height_1010_ed8b(((char *)((unsigned char *)(o) + 0x8)));
}
