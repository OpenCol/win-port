/* MSFitWindow: candidate for 1078:221b (76 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: MSFitWindow
 *
 * Moves and resizes the window to the given rectangle.
 *
 * RENAMED 2026-09-27 from win_move_to_rect_1078_221b: the Mac build's MSFitWindow(GrafPort*, Rect*): MoveWindow to the rectangle, at window level.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSFitWindow(MSWin *win, RECT *r)
{
    if (win != 0)
        MoveWindow(win->hwnd, r->left, r->top,
                   rect_width_1078_33f9(r), rect_height_1078_3418(r), 1);
}
