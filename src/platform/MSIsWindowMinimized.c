/* MSIsWindowMinimized: candidate for 1078:217b (45 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * IsIconic on the window's handle (+2); 0 for a null window.
 *
 * RENAMED 2026-09-27 from win_is_iconic_1078_217b: Mac MSIsWindowMinimized(GrafPort*). Layout: Windows 1078 MSMaximizeWindow, win_is_zoomed, win_minimize, 217b, MSRestoreWindow; the Mac has MSIsWindowMinimized immediately before MSRestoreWindow. Callers through its two wrappers: ColChild_DrawAll, OnMove, OnSize and MainWin_OnMove, OnSize (1048:d8ad), and PediaWin_Show (1060:67e4). Those are Mac MSIsWindowMinimized's 6 callers, 6 of 6. Arity 1. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "records.h"

int MSIsWindowMinimized(MSWin *win)
{
    if (win != 0)
        return IsIconic(win->hwnd);
    else
        return 0;
}

