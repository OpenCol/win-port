/* win_set_cursor_1078_2986 -- shipped at 1078:2986.
 *
 * Ported by tools/port.py from matched/platform/win_set_cursor_1078_2986.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetCursor(col_u16 hcursor);
#endif

void win_set_cursor_1078_2986(MSWin *w, col_i16 cur)
{
    if (w == 0)
        return;
    if (cur != 0) {
        w->hcursor = cur;
        SetCursor(cur);
    }
}
