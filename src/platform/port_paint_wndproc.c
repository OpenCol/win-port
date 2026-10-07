/* PORT: this file's own declarations of port_blit_or_copy removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 55 of the imported file. */
/* port_paint_wndproc: candidate for 1070:bb97 (224 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The window procedure `subclass_control_1070_bb63` installs, and the pair is what makes a
 * plain Windows control paint itself out of a WinG port: that function stores the port in
 * window word 0x10 and points GWL_WNDPROC here, and this one reads word 0x10 back.
 *
 * A window with nothing in word 0x10 is handed straight to `port_wndproc` -- so the
 * subclass is inert until the port is installed, and the test is a 32-bit compare of the
 * whole pointer.
 *
 * WM_PAINT blits the port over the update rectangle, but only while the window is visible,
 * and the rectangle is copied field by field out of the PAINTSTRUCT rather than assigned as
 * a struct: four word moves, where `r = ps.rcPaint` would go through `F_SCOPY`.
 *
 * WM_ERASEBKGND returns 1 without erasing -- the port covers the whole client area, so
 * erasing it first would only flicker. Everything else goes to `dialog_wndproc`.
 *
 * 224 bytes and not the census 213: the last `return 0;` costs a second, unreachable
 * epilogue, and a `RETF 0x0A` frame's epilogue is eleven bytes (rule 44).
 *
 * `port_wndproc` is reached by the four-byte form and is a stub here; `dialog_wndproc` by
 * the five-byte NOP form and is `extern` (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
typedef struct { char pad[1]; } MagicWin;
#include "records.h"


/* same object: 4-byte `push cs; call ` */

long port_paint_wndproc(unsigned hwnd, unsigned msg, unsigned wp,
                                             long lp)
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
            port_blit_or_copy((Port *)((MagicWin *)p), &r);
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
