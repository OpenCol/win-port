/* close_dialog_at_26_1060_612a -- shipped at 1060:612a.
 *
 * Ported by tools/port.py from matched/game/close_dialog_at_26_1060_612a.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_dialog_1060_6b25(void *p);

/* same object: 4-byte `push cs; call ` */


void close_dialog_at_26_1060_612a(Obj *o)
{
    close_dialog_1060_6b25(((char *)((unsigned char *)(o) + 0x26)));
}
