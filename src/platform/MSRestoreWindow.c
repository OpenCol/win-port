/* MSRestoreWindow: candidate for 1078:21a8 (35 bytes).  Verdict: RELOCATABLE.
 * symbol: MSRestoreWindow
 *
 * Restores the window, ShowWindow with SW_RESTORE (9); a null window is ignored. The same
 * 35 bytes as MSHideWindow and MSMaximizeWindow with a different command constant.
 */

#include "colonization.h"

#include "records.h"

void MSRestoreWindow(MSWin *win)
{
    if (win != 0)
        ShowWindow(win->hwnd, 9);
}
