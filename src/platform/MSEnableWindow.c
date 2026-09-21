/* MSEnableWindow -- shipped at 1078:1d00.
 *
 * Ported by tools/port.py from matched/platform/MSEnableWindow.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EnableWindow(col_u16 hwnd, col_i16 on);
#endif

void MSEnableWindow(MSWin *w, col_i16 on)
{
    if (w != 0)
        EnableWindow(w->hwnd, on);
}
