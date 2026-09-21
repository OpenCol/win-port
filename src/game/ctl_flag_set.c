/* ctl_flag_set -- shipped at 1058:c1dd.
 *
 * Ported by tools/port.py from matched/game/ctl_flag_set.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void invalidate_hwnd(col_u16 hwnd);

void ctl_flag_set(Ctl *c)
{
    c->state = 1;
    invalidate_hwnd(c->hbutton);
}
