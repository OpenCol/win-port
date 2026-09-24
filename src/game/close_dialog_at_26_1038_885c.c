/* close_dialog_at_26_1038_885c -- shipped at 1038:885c.
 *
 * Ported by tools/port.py from matched/game/close_dialog_at_26_1038_885c.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_dialog_1038_8aec(void *p);

/* same object: 4-byte `push cs; call ` */


void close_dialog_at_26_1038_885c(Obj *o)
{
    close_dialog_1038_8aec(((char *)((unsigned char *)(o) + 0x26)));
}
