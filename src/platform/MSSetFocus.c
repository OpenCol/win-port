/* MSSetFocus: candidate for 1078:2a30 (49 bytes).  Verdict: RELOCATABLE.
 *
 * SetFocus and BringWindowToTop on the window's handle (+2). A null window returns.
 */

#include "colonization.h"

#include "records.h"

void MSSetFocus(MSWin *win)
{
    if (win == 0)
        return;
    SetFocus(win->hwnd);
    BringWindowToTop(win->hwnd);
}
