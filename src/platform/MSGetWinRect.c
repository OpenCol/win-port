/* MSGetWinRect -- shipped at 1078:2459.
 *
 * Ported by tools/port.py from matched/platform/MSGetWinRect.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetWindowRect(col_u16 hwnd, RECT *r);;
#endif

void MSGetWinRect(MSWin *w, RECT *r)
{
    if (w == 0)
        return;
    GetWindowRect(w->hwnd, r);
}
