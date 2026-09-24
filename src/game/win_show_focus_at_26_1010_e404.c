/* win_show_focus_at_26_1010_e404 -- shipped at 1010:e404.
 *
 * Ported by tools/port.py from matched/game/win_show_focus_at_26_1010_e404.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_show_and_focus_1010_ecec(void *p);

/* same object: 4-byte `push cs; call ` */


void win_show_focus_at_26_1010_e404(Obj *o)
{
    win_show_and_focus_1010_ecec(((char *)((unsigned char *)(o) + 0x26)));
}
