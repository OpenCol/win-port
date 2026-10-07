/* win_client_to_screen: candidate for 1078:2593 (81 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Converts a client point to screen coordinates through the window's HWND at +2, returning
 * the result through two `int *` out-parameters rather than a POINT. A null window
 * pointer returns without touching either.
 *
 * The POINT is a local pair: the two ints are copied into `[bp-6]`/`[bp-4]`, `ClientToScreen`
 * is handed `SS:&pt`, and the members are copied back out. That is the shape of a caller
 * that has x and y as separate variables, which is why the wrapper exists at all.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void win_client_to_screen(MSWin *win, int x, int y, int *px, int *py)
{
    POINT pt;

    if (win == 0)
        return;
    pt.x = x;
    pt.y = y;
    ClientToScreen(win->hwnd, &pt);
    *px = pt.x;
    *py = pt.y;
}
