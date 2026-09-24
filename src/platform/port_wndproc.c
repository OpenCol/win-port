/* port_wndproc -- shipped at 1070:a156.
 *
 * Ported by tools/port.py from matched/platform/port_wndproc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_current_window;              DGROUP:0x2ae0
 *
 * The file's own PortInfo dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
PortInfo * get_p_22_1070_dc44(void *p);
col_i16 port_height_1070_dbe6(void *port);
col_i16 port_width_1070_dbc7(void *port);
col_i16 rect_height(RECT *r);
col_i16 rect_width(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_i16 pascal IsIconic(col_u16 hwnd);
extern col_i16 pascal IsWindowVisible(col_u16 hwnd);
extern col_i16 pascal WinGStretchBlt(col_u16 dst, col_i16 dx, col_i16 dy, col_i16 dw, col_i16 dh, col_u16 src,
                                     col_i16 sx, col_i16 sy, col_i16 sw, col_i16 sh);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 idx);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "w16const.h"
extern void CopyToScreen(void *port, void *win, RECT *sr, RECT *dr);
extern void win_resize(void *port, col_i16 w, col_i16 h);
extern col_i16 GRPort_Init_wh(void *p, col_i16 w, col_i16 h);

/* 5-byte NOP form, so extern (rule 47) */
extern col_i32 pascal dialog_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wparam,
                                                col_u32 lparam);

/* same object: 4-byte `push cs; call ` (rule 47) */






col_i32 pascal port_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wparam,
                                       col_u32 lparam)
{
    void *port;
    RECT rc;
    col_u16 hdc;
    col_u16 srcdc;
    RECT dst;
    PAINTSTRUCT ps;

    port = (void *)GetWindowLong(hwnd, 0xc);
    if (port == 0)
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    switch (msg) {
    case WM_PAINT:
        if (IsIconic(hwnd)) {
            hdc = BeginPaint(hwnd, &ps);
            rc = ps.rcPaint;
            SetRect(&dst, 0, 0, 0, 0);
            if (port_width_1070_dbc7(port) < port_height_1070_dbe6(port)) {
                dst.right = port_width_1070_dbc7(port) * rect_width(&rc) / port_height_1070_dbe6(port);
                dst.bottom = rect_height(&rc);
            } else {
                dst.right = rect_width(&rc);
                dst.bottom = port_height_1070_dbe6(port) * rect_height(&rc) / port_width_1070_dbc7(port);
            }
            srcdc = get_p_22_1070_dc44(port)->hdc;
            WinGStretchBlt(hdc, (rect_width(&rc) - rect_width(&dst)) >> 1,
                           (rect_height(&rc) - rect_height(&dst)) >> 1, dst.right, dst.bottom, srcdc, 0, 0,
                           port_width_1070_dbc7(port), port_height_1070_dbe6(port));
            EndPaint(hwnd, &ps);
        } else if (IsWindowVisible(hwnd)) {
            BeginPaint(hwnd, &ps);
            rc = ps.rcPaint;
            CopyToScreen(port, port != 0 ? (char *)port + 0x26 : (char *)port, &rc, &rc);
            EndPaint(hwnd, &ps);
        }
        break;
    case WM_SIZE:
        g_current_window = port != 0 ? (char *)port + 0x26 : (char *)port;
        if (!IsIconic(hwnd))
            win_resize(port, (col_u16)lparam, (col_u16)(lparam >> 16));
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    case WM_DESTROY:
        GRPort_Init_wh(port, 0, 0);
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    default:
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
