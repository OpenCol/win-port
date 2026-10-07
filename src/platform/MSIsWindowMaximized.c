/* MSIsWindowMaximized: candidate for 1078:212b (45 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * IsZoomed on the window's handle (+2); 0 for a null window.
 *
 * RENAMED 2026-09-27 from win_is_zoomed_1078_212b: Mac MSIsWindowMaximized(GrafPort*). Layout: Windows 1078 MSMaximizeWindow, 212b; the Mac has MSMaximizeWindow, MSIsWindowMaximized adjacent. Callers through its wrapper 1048:d8d2: ColChild_DrawAll, OnMove and OnSize, which are Mac MSIsWindowMaximized's 3 callers exactly. Arity 1. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "records.h"

int MSIsWindowMaximized(MSWin *win)
{
    if (win != 0)
        return IsZoomed(win->hwnd);
    else
        return 0;
}

