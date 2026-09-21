/* win_point_in_parent -- shipped at 1078:25e4.
 *
 * Ported by tools/port.py from matched/platform/win_point_in_parent.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetParent(col_u16 hwnd);
extern void pascal MapWindowPoints(col_u16 from, col_u16 to, POINT *p, col_u16 n);
#endif

void win_point_in_parent(Win *w, col_i16 x, col_i16 y, col_i16 *px, col_i16 *py)
{
    POINT pt;

    if (w == 0)
        return;
    pt.x = x;
    pt.y = y;
    MapWindowPoints(w->hdc, GetParent(w->hdc), &pt, 1);
    *px = pt.x;
    *py = pt.y;
}
