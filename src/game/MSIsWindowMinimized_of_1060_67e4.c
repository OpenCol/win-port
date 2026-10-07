/* MSIsWindowMinimized_of_1060_67e4: candidate for 1060:67e4 (37 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether the window the object holds at +6 is minimised.
 *
 * RENAMED 2026-09-27 from win_is_iconic_1060_67e4: Wrapper of MSIsWindowMinimized (1078:217b), the second copy. Caller: PediaWin_Show, which is Mac PediaWin::Show, a caller of MSIsWindowMinimized. Name length exactly 32. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int MSIsWindowMinimized(void *win);

int MSIsWindowMinimized_of_1060_67e4(DialogWin *o)
{
    return MSIsWindowMinimized((MSWin *)o->win);
}
