/* MSGetSizeParent_of: candidate for 1048:d674 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The rectangle of the window the object holds at +6, in its parent's coordinates.
 *
 * RENAMED 2026-09-27 from win_rect_in_parent_1048_d674: Wrapper of MSGetSizeParent (1078:2644) on the object's +6 window. It is a singleton, so the name is MSGetSizeParent_of. Callers: exactly the Mac MSGetSizeParent callers, 4 of 4. The Mac has no wrapper; ColChild calls MSGetSizeParent directly. mac_bind's proposal (bare MSGetSizeParent here) is taken in the family form. Convention: the 1078 body takes the window (MSWin *), as the Mac takes GrafPort*, so the body carries the MS name, like MSShowWindow/MSHideWindow/MSSetFocus. The per-module wrappers that hand an object's +6 window to it are MSName_of (singleton) or MSName_of_<seg>_<off> (several copies), as MSShowWindow_of_1060_8623 and MSGetWinRect_of are.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void MSGetSizeParent(MSWin *win, RECT *r);

void MSGetSizeParent_of(Screen *o, RECT *r)
{
    MSGetSizeParent((MSWin *)o->win, r);
}
