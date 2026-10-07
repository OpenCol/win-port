/* nonclient_height: candidate for 1078:24f5 (113 bytes).  Verdict: RELOCATABLE.
 * symbol: nonclient_height
 *
 * The height of the window's non-client frame: the window rectangle's height less the client rectangle's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int nonclient_width(MSWin *win);

int nonclient_height(MSWin *win)
{
    int outer;
    int inner;
    RECT r;

    if (win == 0)
        return 0;
    GetWindowRect(win->hwnd, &r);
    outer = rect_height_1078_3418(&r);
    GetClientRect(win->hwnd, &r);
    inner = rect_height_1078_3418(&r);
    return outer - inner;
}
