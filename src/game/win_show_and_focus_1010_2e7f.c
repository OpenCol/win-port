/* win_show_and_focus_1010_2e7f -- shipped at 1010:2e7f.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1010_2e7f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1010_29c1(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1010_2e7f(void *o)
{
    MSShowWindow_of_1010_29c1(o);
    focus_marked_ctl(o);
}
