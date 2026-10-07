/* MSSetWinTitle: candidate for 1078:1fd1 (39 bytes).  Verdict: RELOCATABLE.
 *
 * SetWindowText on the window's handle (+2) for a non-null window.
 */

#include "colonization.h"

#include "records.h"

void MSSetWinTitle(MSWin *win, const char *s)
{
    if (win != 0)
        SetWindowText(win->hwnd, s);
}
