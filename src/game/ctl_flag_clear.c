/* ctl_flag_clear -- shipped at 1058:c199.
 *
 * Ported by tools/port.py from matched/game/ctl_flag_clear.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void invalidate_hwnd(col_u16 hwnd);

void ctl_flag_clear(Ctl *c)
{
    c->state = 0;
    invalidate_hwnd(c->hbutton);
}
