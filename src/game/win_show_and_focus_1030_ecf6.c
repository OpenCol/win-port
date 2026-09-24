/* win_show_and_focus_1030_ecf6 -- shipped at 1030:ecf6.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1030_ecf6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1030_ef7f(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1030_ecf6(void *o)
{
    MSShowWindow_of_1030_ef7f(o);
    focus_marked_ctl(o);
}
