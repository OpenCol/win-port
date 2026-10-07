/* MSValidRect: candidate for 1078:21cb (39 bytes).  Verdict: RELOCATABLE.
 *
 * ValidateRect on the window's handle (+2) for a non-null window.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSValidRect(MSWin *win, RECT *rect)
{
    if (win != 0)
        ValidateRect(win->hwnd, rect);
}
