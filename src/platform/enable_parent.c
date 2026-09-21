/* enable_parent -- shipped at 1078:2aa8.
 *
 * Ported by tools/port.py from matched/platform/enable_parent.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EnableWindow(col_u16 hwnd, col_i16 enable);;
extern col_i16 pascal IsWindow(col_u16 hwnd);;
extern col_u16 pascal GetParent(col_u16 hwnd);;
#endif

void enable_parent(MSWin *w)
{
    if (w == 0)
        return;
    if (!IsWindow(GetParent(w->hwnd)))
        return;
    EnableWindow(GetParent(w->hwnd), 1);
}
