/* win_show_and_focus_1058_bc47 -- shipped at 1058:bc47.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1058_bc47.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetFocus_of_1058_c508(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1058_bc47(void *o)
{
    MSSetFocus_of_1058_c508(o);
    focus_marked_ctl(o);
}
