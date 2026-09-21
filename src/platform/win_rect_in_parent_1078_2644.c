/* win_rect_in_parent_1078_2644 -- shipped at 1078:2644.
 *
 * Ported by tools/port.py from matched/platform/win_rect_in_parent_1078_2644.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetParent(col_u16 hwnd);
extern void pascal GetWindowRect(col_u16 hwnd, RECT *r);
extern void pascal MapWindowPoints(col_u16 from, col_u16 to, void *pts, col_i16 n);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

void win_rect_in_parent_1078_2644(MSWin *w, RECT *r)
{
    RECT t;

    if (w == 0)
        return;
    GetWindowRect(w->hwnd, r);
    t.left = r->left;
    t.top = r->top;
    t.right = r->right;
    t.bottom = r->bottom;
    MapWindowPoints(0, GetParent(w->hwnd), &t, 2);
    SetRect(r, t.left, t.top, t.right, t.bottom);
}
