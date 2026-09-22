/* anim_stop -- shipped at 1068:2715.
 *
 * Ported by tools/port.py from matched/gfx/anim_stop.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSPositionFile_1068_9705(void *port, col_i32 data);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void ResetTimer(col_i16 id);

void anim_stop(Obj *o)
{
    o->cur = o->saved;
    o->a = o->b = 0;
    MSPositionFile_1068_9705(((char *)((unsigned char *)(o) + 0xa6)), o->data);
    if (o->timer != -1) {
        ResetTimer(o->timer);
        o->timer = -1;
    }
}
