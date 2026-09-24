/* win_show_and_focus_1068_e4ee -- shipped at 1068:e4ee.
 *
 * Ported by tools/port.py from matched/gfx/win_show_and_focus_1068_e4ee.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1068_e66f(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1068_e4ee(void *o)
{
    MSShowWindow_of_1068_e66f(o);
    focus_marked_ctl(o);
}
