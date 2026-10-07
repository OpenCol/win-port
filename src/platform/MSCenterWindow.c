/* MSCenterWindow: candidate for 1078:2267 (372 bytes).  Verdict: RELOCATABLE.
 *
 * Centres the window in its parent (or the screen), offset by (dx, dy). For a parented
 * window the parent's client origin is mapped to screen coordinates; a window with bit 1 of
 * its flag byte +0x25 set is positioned relative to the parent's window rectangle instead and
 * never negative.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void MSCenterWindow(MSWin *win, int dx, int dy)
{
    int x;
    int y;
    unsigned parent;
    RECT wr;
    RECT pr;
    int pw;
    int ph;
    POINT pt;

    if (win == 0)
        return;
    GetWindowRect(win->hwnd, &wr);
    if ((parent = GetParent(win->hwnd)) != 0) {
        GetWindowRect(parent, &pr);
        pw = rect_width_1078_33f9(&pr);
        ph = rect_height_1078_3418(&pr);
        x = (pw - rect_width_1078_33f9(&wr)) / 2;
        y = (ph - rect_height_1078_3418(&wr)) / 2;
        pt.x = 0;
        pt.y = 0;
        MapWindowPoints(parent, 0, &pt, 1);
        if ((win->flags & 0x200) == 0) {
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
    MoveWindow(win->hwnd, x + dx, y + dy, rect_width_1078_33f9(&wr), rect_height_1078_3418(&wr), 1);
}
