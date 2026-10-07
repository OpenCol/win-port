/* MSShowWindow: candidate for 1078:1f75 (57 bytes).  Verdict: RELOCATABLE.
 *
 * ShowWindow(hwnd, SW_SHOW) and, when bit 1 of the flag byte at +0x25 is set,
 * BringWindowToTop.
 */

#include "colonization.h"

#include "records.h"

void MSShowWindow(MSWin *win)
{
    if (win != 0) {
        ShowWindow(win->hwnd, 5);
        if (win->flags & 0x200)
            BringWindowToTop(win->hwnd);
    }
}
