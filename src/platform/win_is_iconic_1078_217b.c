/* win_is_iconic_1078_217b -- shipped at 1078:217b.
 *
 * Ported by tools/port.py from matched/platform/win_is_iconic_1078_217b.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IsIconic(col_u16 hwnd);
#endif

col_i16 win_is_iconic_1078_217b(MSWin *w)
{
    if (w != 0)
        return IsIconic(w->hwnd);
    else
        return 0;
}
