/* MSGetWinSize: candidate for 1078:2459 (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * GetWindowRect on the window's handle (+2); nothing for a null window.
 *
 * RENAMED 2026-09-27 from MSGetWinRect: Mac MSGetWinSize(GrafPort*, Rect*). MSGetWinRect is not a Mac symbol (the Mac table has only MSGetWinSize). It was a descriptive MS-style name. Layout: Windows 1078 MSFitWindow, MSCenterWindow, win_client_width, win_client_height, 2459; the Mac MS file has MSFitWindow, MSCenterWindow, MSWM_HAS_TITLEBAR, MSWinWidth, MSWinHeight, MSGetWinSize, the same order. Callers through its wrappers: win_screen_rect's callers MainWin_Init, OnMove and OnSize are Mac MSGetWinSize's 3 callers exactly (MSGetWinRect_of adds ask_choice). Arity 2. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSGetWinSize(MSWin *win, RECT *r)
{
    if (win == 0)
        return;
    GetWindowRect(win->hwnd, r);
}
