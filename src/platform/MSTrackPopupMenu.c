/* MSTrackPopupMenu -- shipped at 1070:d996.
 *
 * Ported by tools/port.py from matched/platform/MSTrackPopupMenu.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal TrackPopupMenu(col_u16 hmenu, col_u16 flags, col_i16 x, col_i16 y,
                                     col_i16 reserved, col_u16 hwnd, const void *r);
extern void pascal ClientToScreen(col_u16 hwnd, POINT *p);
#endif

void MSTrackPopupMenu(col_u16 hmenu, MSWin *w, col_i16 x, col_i16 y)
{
    POINT pt;

    pt.x = x;
    pt.y = y;
    ClientToScreen(w->hwnd, &pt);
    TrackPopupMenu(hmenu, 2, pt.x, pt.y, 0, w->hwnd, 0);
}
