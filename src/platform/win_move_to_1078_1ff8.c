/* win_move_to_1078_1ff8: candidate for 1078:1ff8 (97 bytes).  Verdict: RELOCATABLE.
 * symbol: win_move_to_1078_1ff8
 *
 * Moves the window to a point without resizing it, reading the current size back from GetWindowRect.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void win_move_to_1078_1ff8(MSWin *win, int x, int y)
{
    RECT r;
    if (win == 0)
        return;
    GetWindowRect(win->hwnd, &r);
    MoveWindow(win->hwnd, x, y, rect_width_1078_33f9(&r), rect_height_1078_3418(&r), 1);
}
