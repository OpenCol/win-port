/* win_point_in_parent: candidate for 1078:25e4 (96 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The twin of `win_client_to_screen` 81 bytes above it, mapping into the PARENT instead of
 * the screen: `MapWindowPoints(w->hwnd, GetParent(w->hwnd), &pt, 1)` where the other calls
 * `ClientToScreen`. Everything else is identical, down to the local POINT built from two
 * separate ints and copied back out through two `int *`.
 *
 * `MSGetSizeParent` is the next function along and does the same for a
 * rectangle.
 *
 * The HWND is fetched twice -- once for the source and once to ask for the parent -- which
 * is two `LES BX,[bp+6]` where a temporary would have done.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void win_point_in_parent(MSWin *win, int x, int y, int *px, int *py)
{
    POINT pt;

    if (win == 0)
        return;
    pt.x = x;
    pt.y = y;
    MapWindowPoints(win->hwnd, GetParent(win->hwnd), &pt, 1);
    *px = pt.x;
    *py = pt.y;
}
