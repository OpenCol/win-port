/* win_set_client_size_1078_2059 -- shipped at 1078:2059.
 *
 * Ported by tools/port.py from matched/platform/win_set_client_size_1078_2059.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 win_client_height_1078_241a(MSWin *w);
col_i16 win_client_width_1078_23db(MSWin *w);
void win_set_menu(MSWin *w, col_u16 hmenu);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetSystemMetrics(col_i16 index);;
extern col_i16 pascal GetWindowRect(col_u16 hwnd, RECT *r);;
extern col_i16 pascal MapWindowPoints(col_u16 from, col_u16 to, POINT *p, col_u16 n);;
extern col_i16 pascal MoveWindow(col_u16 hwnd, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 repaint);;
extern col_i16 pascal SetMenu(col_u16 hwnd, col_u16 hmenu);;
extern col_u16 pascal GetParent(col_u16 hwnd);;
extern void pascal GetClientRect(col_u16 hwnd, RECT *r);;
#endif

extern col_i16 nonclient_width(MSWin *w);
extern col_i16 nonclient_height(MSWin *w);





void win_set_client_size_1078_2059(MSWin *w, col_i16 cw, col_i16 ch)
{
    RECT r;
    col_i16 fw;
    col_i16 fh;
    col_u16 parent;
    POINT p;

    if (w == 0)
        return;
    GetWindowRect(w->hwnd, &r);
    fw = nonclient_width(w) + cw;
    fh = nonclient_height(w) + ch;
    if ((parent = GetParent(w->hwnd)) != 0) {
        p.x = 0;
        p.y = 0;
        MapWindowPoints(parent, 0, &p, 1);
        r.left -= p.x;
        r.top -= p.y;
    }
    MoveWindow(w->hwnd, r.left, r.top, fw, fh, 1);
}
