/* win_show_and_focus_1058_c4df -- shipped at 1058:c4df.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1058_c4df.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSShowWindow_of_1058_c6e8(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1058_c4df(void *o)
{
    MSShowWindow_of_1058_c6e8(o);
    focus_marked_ctl(o);
}
