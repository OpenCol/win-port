/* win_show_and_focus_1010_ecec -- shipped at 1010:ecec.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1010_ecec.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1010_f0a0(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1010_ecec(void *o)
{
    MSShowWindow_of_1010_f0a0(o);
    focus_marked_ctl(o);
}
