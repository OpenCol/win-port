/* MSMaximizeWindow: candidate for 1078:2108 (35 bytes).  Verdict: RELOCATABLE.
 * symbol: MSMaximizeWindow
 *
 * Maximizes the window, ShowWindow with SW_MAXIMIZE (3); a null window is ignored.
 */

#include "colonization.h"

#include "records.h"

void MSMaximizeWindow(MSWin *win)
{
    if (win != 0)
        ShowWindow(win->hwnd, 3);
}
