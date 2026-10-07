/* nonclient_width: candidate for 1078:2484 (113 bytes).  Verdict: RELOCATABLE.
 * symbol: nonclient_width
 *
 * The width of the window's non-client frame: the window rectangle's width less the client rectangle's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

int nonclient_width(MSWin *win)
{
    int outer;
    int inner;
    RECT r;

    if (win == 0)
        return 0;
    GetWindowRect(win->hwnd, &r);
    outer = rect_width_1078_33f9(&r);
    GetClientRect(win->hwnd, &r);
    inner = rect_width_1078_33f9(&r);
    return outer - inner;
}

extern int nonclient_height(MSWin *win);
