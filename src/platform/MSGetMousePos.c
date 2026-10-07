/* MSGetMousePos: candidate for 1078:2c91 (65 bytes).  Verdict: RELOCATABLE.
 *
 * Cursor position in the window's client coordinates through two out-pointers.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSGetMousePos(MSWin *win, int *x, int *y)
{
    POINT p;

    GetCursorPos(&p);
    ScreenToClient(win->hwnd, &p);
    *x = p.x;
    *y = p.y;
}
