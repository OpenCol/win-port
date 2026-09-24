/* port_paint_wndproc -- shipped at 1070:bb97.
 *
 * Ported by tools/port.py from matched/platform/port_paint_wndproc.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 pascal port_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_i16 pascal IsWindowVisible(col_u16 hwnd);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
#endif

#include "w16const.h"
extern void port_blit_or_copy(Port *p, RECT *r);
extern col_i32 pascal dialog_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_u32 lparam);

/* same object: 4-byte `push cs; call ` */


col_i32 pascal port_paint_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wp,
                                             col_i32 lp)
{
    Port *p;
    RECT r;
    PAINTSTRUCT ps;

    p = (Port *)GetWindowLong(hwnd, 0x10);
    if (p == 0)
        return port_wndproc(hwnd, msg, wp, lp);
    switch (msg) {
    case WM_PAINT:
        if (IsWindowVisible(hwnd)) {
            BeginPaint(hwnd, &ps);
            r.left = ps.rcPaint.left;
            r.top = ps.rcPaint.top;
            r.right = ps.rcPaint.right;
            r.bottom = ps.rcPaint.bottom;
            port_blit_or_copy(p, &r);
            EndPaint(hwnd, &ps);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    default:
        return dialog_wndproc(hwnd, msg, wp, lp);
    }
    return 0;
}
