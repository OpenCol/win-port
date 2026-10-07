/* MSGetWinSize_of_1068_df03: candidate for 1068:df03 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `MSGetWinSize` on the window the object holds at +6, with the caller's pointer passed
 * through -- the same one-line shape as `win_set_focus`, one argument wider.
 *
 * RENAMED 2026-09-27 from MSGetWinRect_of: Wrapper of MSGetWinSize (1078:2459), the second copy. It follows its body's rename. Caller: ask_choice. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void MSGetWinSize(MSWin *win, RECT *r);

void MSGetWinSize_of_1068_df03(DialogWin *o, void *p)
{
    MSGetWinSize((MSWin *)o->win, (RECT *)p);
}
