/* MSSetCursor: candidate for 1078:294a (60 bytes).  Verdict: RELOCATABLE.
 *
 * Records cursor `cur` on the window through MSSetCursor_id and, when `now`, makes the
 * window's cursor handle (+0xc) current. A null window returns.
 */

#include "colonization.h"

#include "records.h"
extern void MSSetCursor_id(MSWin *win, int cur);

void MSSetCursor(MSWin *win, int cur, int now)
{
    if (win == 0)
        return;
    MSSetCursor_id(win, cur);
    if (now)
        SetCursor(win->hcursor);
}
