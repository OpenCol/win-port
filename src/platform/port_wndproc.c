/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: lParam's words as Borland's 16-bit casts took them -- i16 for coordinates, sizes and positions, u16 for handles, ids and codes; (unsigned)/(int) keep 32 bits here (W4). */
/* PORT: this file's own declarations of GRPort_GetWin_1070_dc44, GameWin_Resize removed: the header declares them from the definition (W1 step 3). */
/* port_wndproc: candidate for 1070:a156 (770 bytes).  Verdict: RELOCATABLE.
 *
 * The window procedure of a window that shows a graphics port, the port kept in window long 12.
 * It sits on top of `dialog_wndproc`, which gets every message it does not finish and
 * every message before the port is attached.
 *
 *   WM_PAINT (0xf): minimised, the whole port is stretched with WinGStretchBlt into the icon,
 *       scaled to fit one dimension while keeping its proportions; otherwise, when the window is
 *       visible, the invalid rectangle is copied from the port to the screen. The port's own DC
 *       comes from the record at +0x22.
 *   WM_SIZE (5): the window becomes the current window (DGROUP:0x2ae0) and, unless minimised, the
 *       port is resized to the new client size before the message is passed on.
 *   WM_DESTROY (2): the port is emptied to 0 by 0 and the message passed on.
 *
 * The icon's target size is built in a RECT set to zero and then given only its right and bottom,
 * so its width and height are the size, and the image is centred by half the difference from the
 * paint rectangle. The switch bodies are in the source's order -- paint, size, destroy,
 * default (rule 36).
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#include "classes.h"
#include "records.h"

extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

/* 5-byte NOP form, so extern (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int rect_width(RECT *r);
extern int rect_height(RECT *r);

long port_wndproc(unsigned hwnd, unsigned msg, unsigned wparam,
                                       uintptr_t lparam)
{
    GRPort *port;
    RECT rc;
    unsigned hdc;
    unsigned srcdc;
    RECT dst;
    PAINTSTRUCT ps;

    port = (GRPort *)GetWindowLong(hwnd, 0xc);
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
            srcdc = GRPort_GetWin_1070_dc44(port)->hdc;
            WinGStretchBlt(hdc, (rect_width(&rc) - rect_width(&dst)) >> 1,
                           (rect_height(&rc) - rect_height(&dst)) >> 1, dst.right, dst.bottom, srcdc, 0, 0,
                           port_width_1070_dbc7(port), port_height_1070_dbe6(port));
            EndPaint(hwnd, &ps);
        } else if (IsWindowVisible(hwnd)) {
            BeginPaint(hwnd, &ps);
            rc = ps.rcPaint;
            CopyToScreen(port, (Screen *)(port != 0 ? (char *)port + 0x26 : (char *)port), &rc, &rc);
            EndPaint(hwnd, &ps);
        }
        break;
    case WM_SIZE:
        g_current_window = port != 0 ? (char *)port + 0x26 : (char *)port;
        if (!IsIconic(hwnd))
            GameWin_Resize((GameWin *)port, (i16)lparam, (i16)(lparam >> 16));
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    case WM_DESTROY:
        GRPort_Init_wh(port, 0, 0);
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    default:
        return dialog_wndproc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
