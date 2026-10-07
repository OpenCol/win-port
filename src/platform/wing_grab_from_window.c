/* wing_grab_from_window: candidate for 1070:c949 (184 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_grab_from_window
 *
 * Copies the rectangle from a window's screen DC into a port's DC (the window's palette
 * selected and realised first when it has one and DGROUP:0x3380 is 1); nothing when either
 * object is null. rect_width/rect_height are defined here because the shipped calls to them
 * are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_grab_from_window(GrafRec *dst, MSWin *win, RECT *r)
{
    unsigned dc;
    int x;
    int w;
    int h;
    int y;

    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    if (dst != 0 && win != 0) {
        dc = GetDC(win->hwnd);
        if (win->hpal != 0 && g_palette_mode == 1) {
            SelectPalette(dc, win->hpal, 0);
            RealizePalette(dc);
        }
        WinGBitBlt(dst->hdc, x, y, w, h, dc, x, y);
        ReleaseDC(win->hwnd, dc);
    }
}
