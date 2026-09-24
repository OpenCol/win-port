/* close_dialog_at_26_1060_87b1 -- shipped at 1060:87b1.
 *
 * Ported by tools/port.py from matched/game/close_dialog_at_26_1060_87b1.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_dialog_1060_8ae2(void *p);

/* same object: 4-byte `push cs; call ` */


void close_dialog_at_26_1060_87b1(Obj *o)
{
    close_dialog_1060_8ae2(((char *)((unsigned char *)(o) + 0x26)));
}
