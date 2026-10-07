/* PORT: this file's own declarations of MSIsWindowMinimized removed: the header declares it from the definition (W1 step 3). */
/* MSIsWindowMinimized_of_1048_d8ad: candidate for 1048:d8ad (37 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether the window the object holds at +6 is minimised.
 *
 * RENAMED 2026-09-27 from win_is_iconic_1048_d8ad: Wrapper of MSIsWindowMinimized (1078:217b), one of two copies. Callers: ColChild_DrawAll, OnMove, OnSize, MainWin_OnMove and OnSize. Name length exactly 32. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

int MSIsWindowMinimized_of_1048_d8ad(Screen *o)
{
    return MSIsWindowMinimized((MSWin *)o->win);
}
