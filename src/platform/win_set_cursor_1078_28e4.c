/* win_set_cursor_1078_28e4: candidate for 1078:28e4 (54 bytes).  Verdict: RELOCATABLE.
 *
 * Remembers a non-null cursor at +0xc of the window and, with `now` set,
 * selects it.
 */

#include "colonization.h"

#include "records.h"

void win_set_cursor_1078_28e4(MSWin *win, int cur, int now)
{
    if (win == 0)
        return;
    if (cur != 0) {
        win->hcursor = cur;
        if (now != 0)
            SetCursor(cur);
    }
}
