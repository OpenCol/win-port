/* MSSetWinTitle -- shipped at 1078:1fd1.
 *
 * Ported by tools/port.py from matched/platform/MSSetWinTitle.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetWindowText(col_u16 hwnd, const char *s);
#endif

void MSSetWinTitle(MSWin *w, const char *s)
{
    if (w != 0)
        SetWindowText(w->hwnd, s);
}
