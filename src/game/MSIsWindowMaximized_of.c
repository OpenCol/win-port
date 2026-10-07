/* MSIsWindowMaximized_of: candidate for 1048:d8d2 (37 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether the window the object holds at +6 is maximised.
 *
 * RENAMED 2026-09-27 from win_is_zoomed_1048_d8d2: Wrapper of MSIsWindowMaximized (1078:212b), a singleton. Callers: ColChild_DrawAll, OnMove and OnSize, 3 of 3. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int MSIsWindowMaximized(void *win);

int MSIsWindowMaximized_of(Screen *o)
{
    return MSIsWindowMaximized((MSWin *)o->win);
}
