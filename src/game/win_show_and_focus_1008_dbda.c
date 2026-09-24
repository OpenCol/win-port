/* win_show_and_focus_1008_dbda -- shipped at 1008:dbda.
 *
 * Ported by tools/port.py from matched/game/win_show_and_focus_1008_dbda.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_show(void *o);

extern void focus_marked_ctl(void *o);

/* same object: 4-byte `push cs; call ` */


void win_show_and_focus_1008_dbda(void *o)
{
    win_show(o);
    focus_marked_ctl(o);
}
