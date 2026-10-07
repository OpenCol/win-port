/* invalidate_window_rect: candidate for 1078:21f2 (41 bytes).  Verdict: RELOCATABLE.
 *
 * InvalidateRect(w->hwnd, rect, FALSE) for a non-null window.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void invalidate_window_rect(MSWin *win, void *rect)
{
    if (win != 0)
        InvalidateRect(win->hwnd, (const RECT *)rect, 0);
}
