/* rect_width_at_8_1038_86f0 -- shipped at 1038:86f0.
 *
 * Ported by tools/port.py from matched/game/rect_width_at_8_1038_86f0.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1038_8967(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_width_at_8_1038_86f0(Obj *o)
{
    return rect_width_1038_8967(((char *)((unsigned char *)(o) + 0x8)));
}
