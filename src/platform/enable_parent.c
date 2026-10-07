/* enable_parent: candidate for 1078:2aa8 (71 bytes).  Verdict: RELOCATABLE.
 *
 * Enables the window's parent, after checking the parent is still a window.
 */

#include "colonization.h"

#include "records.h"

void enable_parent(MSWin *win)
{
    if (win == 0)
        return;
    if (!IsWindow(GetParent(win->hwnd)))
        return;
    EnableWindow(GetParent(win->hwnd), 1);
}
