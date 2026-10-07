/* win_set_cursor_1078_2986: candidate for 1078:2986 (48 bytes).  Verdict: RELOCATABLE.
 *
 * Remembers a non-null cursor at +0xc of the window and selects it.
 */

#include "colonization.h"

#include "records.h"

void win_set_cursor_1078_2986(MSWin *win, int cur)
{
    if (win == 0)
        return;
    if (cur != 0) {
        win->hcursor = cur;
        SetCursor(cur);
    }
}
