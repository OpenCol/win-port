/* MSCenterWindow -- shipped at 1078:2267.
 *
 * Ported by tools/port.py from matched/platform/MSCenterWindow.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1078_3418(RECT *r);
col_i16 rect_width_1078_33f9(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetSystemMetrics(col_i16 index);
extern col_i16 pascal MapWindowPoints(col_u16 from, col_u16 to, POINT *pts, col_u16 n);
extern col_u16 pascal GetParent(col_u16 hwnd);
extern void pascal GetWindowRect(col_u16 hwnd, RECT *r);
extern void pascal MoveWindow(col_u16 hwnd, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 repaint);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */


void MSCenterWindow(Win *w, col_i16 dx, col_i16 dy)
{
    col_i16 x;
    col_i16 y;
    col_u16 parent;
    RECT wr;
    RECT pr;
    col_i16 pw;
    col_i16 ph;
    POINT pt;

    if (w == 0)
        return;
    GetWindowRect(w->hdc, &wr);
    if ((parent = GetParent(w->hdc)) != 0) {
        GetWindowRect(parent, &pr);
        pw = rect_width_1078_33f9(&pr);
        ph = rect_height_1078_3418(&pr);
        x = (pw - rect_width_1078_33f9(&wr)) / 2;
        y = (ph - rect_height_1078_3418(&wr)) / 2;
        pt.x = 0;
        pt.y = 0;
        MapWindowPoints(parent, 0, &pt, 1);
        if ((w->flags & 2) == 0) {
            x += pt.x;
            y += pt.y;
        } else {
            x -= pt.x - pr.left;
            y -= pt.y - pr.top;
            if (x < 0)
                x = 0;
            if (y < 0)
                y = 0;
        }
    } else {
        pw = GetSystemMetrics(0);
        ph = GetSystemMetrics(1);
        x = (pw - rect_width_1078_33f9(&wr)) / 2;
        y = (ph - rect_height_1078_3418(&wr)) / 2;
    }
    MoveWindow(w->hdc, x + dx, y + dy, rect_width_1078_33f9(&wr), rect_height_1078_3418(&wr), 1);
}
