/* MSHideWindow: candidate for 1078:1fae (35 bytes).  Verdict: RELOCATABLE.
 * symbol: MSHideWindow
 *
 * Hides the window, ShowWindow with SW_HIDE; a null window is ignored.
 */

#include "colonization.h"

#include "records.h"

void MSHideWindow(MSWin *win)
{
    if (win != 0)
        ShowWindow(win->hwnd, 0);
}
