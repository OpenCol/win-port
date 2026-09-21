/* call_aeaf_at_8 -- shipped at 1060:aafc.
 *
 * Ported by tools/port.py from matched/game/call_aeaf_at_8.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1060_aeaf(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_aeaf_at_8(Obj *o)
{
    return rect_height_1060_aeaf(((char *)((unsigned char *)(o) + 0x8)));
}
