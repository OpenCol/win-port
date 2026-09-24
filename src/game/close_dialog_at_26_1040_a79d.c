/* close_dialog_at_26_1040_a79d -- shipped at 1040:a79d.
 *
 * Ported by tools/port.py from matched/game/close_dialog_at_26_1040_a79d.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_dialog_1040_b052(void *p);

/* same object: 4-byte `push cs; call ` */


void close_dialog_at_26_1040_a79d(Obj *o)
{
    close_dialog_1040_b052(((char *)((unsigned char *)(o) + 0x26)));
}
