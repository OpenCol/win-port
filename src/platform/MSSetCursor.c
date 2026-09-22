/* MSSetCursor -- shipped at 1078:294a.
 *
 * Ported by tools/port.py from matched/platform/MSSetCursor.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetCursor(col_u16 hcursor);
#endif

extern void win_load_cursor_1078_29b6(Win *w, col_i16 cur);


void MSSetCursor(Win *w, col_i16 cur, col_i16 now)
{
    if (w == 0)
        return;
    win_load_cursor_1078_29b6(w, cur);
    if (now)
        SetCursor(w->hcursor);
}
