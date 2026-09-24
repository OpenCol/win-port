/* win_show_and_focus_1010_e4ff -- shipped at 1010:e4ff.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1010_e4ff.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetFocus_of_1010_eb8f(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1010_e4ff(void *o)
{
    MSSetFocus_of_1010_eb8f(o);
    focus_marked_ctl(o);
}
