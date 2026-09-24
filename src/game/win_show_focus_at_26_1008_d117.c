/* win_show_focus_at_26_1008_d117 -- shipped at 1008:d117.
 *
 * Ported by tools/port.py from matched/game/win_show_focus_at_26_1008_d117.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_show_and_focus_1008_dbda(void *p);

/* same object: 4-byte `push cs; call ` */


void win_show_focus_at_26_1008_d117(Obj *o)
{
    win_show_and_focus_1008_dbda(((char *)((unsigned char *)(o) + 0x26)));
}
