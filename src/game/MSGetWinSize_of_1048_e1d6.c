/* MSGetWinSize_of_1048_e1d6: candidate for 1048:e1d6 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The screen rectangle of the window the object holds at +6.
 *
 * RENAMED 2026-09-27 from win_screen_rect: Wrapper of MSGetWinSize (1078:2459), one of two copies (with 1068:df03). Callers: MainWin_Init, OnMove and OnSize, 3 of 3 of Mac MSGetWinSize's. mac_bind proposed the bare name here; it goes to the body, and this wrapper takes the family form. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void MSGetWinSize(MSWin *win, RECT *r);

void MSGetWinSize_of_1048_e1d6(Screen *o, RECT *r)
{
    MSGetWinSize((MSWin *)o->win, r);
}
