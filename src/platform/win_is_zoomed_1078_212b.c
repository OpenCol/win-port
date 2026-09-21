/* win_is_zoomed_1078_212b -- shipped at 1078:212b.
 *
 * Ported by tools/port.py from matched/platform/win_is_zoomed_1078_212b.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IsZoomed(col_u16 hwnd);
#endif

col_i16 win_is_zoomed_1078_212b(MSWin *w)
{
    if (w != 0)
        return IsZoomed(w->hwnd);
    else
        return 0;
}
