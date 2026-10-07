/* MSGetSizeParent: candidate for 1078:2644 (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The window's screen rectangle mapped into its parent's client
 * coordinates (MapWindowPoints from the desktop); nothing for a null window.
 *
 * RENAMED 2026-09-27 from win_rect_in_parent_1078_2644: Mac MSGetSizeParent(GrafPort*, Rect*). The only caller is the ColChild wrapper 1048:d674. That wrapper's callers, ColChild_DrawAll, ColChild_InitCWin_style_ncparent (x2), ColChild_OnMove and ColChild_OnSize, are exactly Mac MSGetSizeParent's 4 callers (DrawAll, InitCWin, OnMove, OnSize); the Mac InitCWin seq has MSGetSizeParent twice. Arity: 2 = (win, r). Body: the window rectangle in its parent's coordinates. Layout: the 1078 order differs from the Mac MS file here (between win_point_in_parent and wing_blit), so layout is silent, not contrary. mac_bind proposed the name for the wrapper; it belongs to this body. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSGetSizeParent(MSWin *win, RECT *r)
{
    RECT t;

    if (win == 0)
        return;
    GetWindowRect(win->hwnd, r);
    t.left = r->left;
    t.top = r->top;
    t.right = r->right;
    t.bottom = r->bottom;
    MapWindowPoints(0, GetParent(win->hwnd), (POINT *)&t, 2);
    SetRect(r, t.left, t.top, t.right, t.bottom);
}
