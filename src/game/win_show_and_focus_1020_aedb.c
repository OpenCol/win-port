/* win_show_and_focus_1020_aedb -- shipped at 1020:aedb.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1020_aedb.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1020_b106(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1020_aedb(void *o)
{
    MSShowWindow_of_1020_b106(o);
    focus_marked_ctl(o);
}
