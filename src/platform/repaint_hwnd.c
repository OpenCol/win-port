/* repaint_hwnd: candidate for 1070:0ae5 (55 bytes).  Verdict: RELOCATABLE.
 *
 * Invalidates the whole client area (erasing) and updates the window.
 */

#include "colonization.h"

#include "w16types.h"

void repaint_hwnd(unsigned hwnd)
{
    RECT r;

    GetClientRect(hwnd, &r);
    InvalidateRect(hwnd, &r, 1);
    UpdateWindow(hwnd);
}
