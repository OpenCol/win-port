/* win_take_focus -- shipped at 1060:62f3.
 *
 * Ported by tools/port.py from matched/game/win_take_focus.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_set_focus(void *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void focus_marked_ctl(void *w);

void win_take_focus(void *w)
{
    win_set_focus(w);
    focus_marked_ctl(w);
}
