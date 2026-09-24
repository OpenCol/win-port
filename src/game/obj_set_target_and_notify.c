/* obj_set_target_and_notify -- shipped at 1060:afa1.
 *
 * Ported by tools/port.py from matched/game/obj_set_target_and_notify.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_select_palette_1060_b07f(Obj *o, void *p);

/* same object: 4-byte `push cs; call ` (rule 47) */

void obj_set_target_and_notify(Obj *o, void *p)
{
    (*((void * *)((unsigned char *)(o) + 0x2))) = p;
    win_select_palette_1060_b07f(o, p);
}
