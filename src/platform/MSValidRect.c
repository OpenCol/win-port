/* MSValidRect -- shipped at 1078:21cb.
 *
 * Ported by tools/port.py from matched/platform/MSValidRect.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal ValidateRect(col_u16 hwnd, RECT *r);
#endif

void MSValidRect(MSWin *w, RECT *r)
{
    if (w != 0)
        ValidateRect(w->hwnd, r);
}
