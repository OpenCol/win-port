/* fame_OnLMouseDown: candidate for 1050:780d (79 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A button handler of module 1050: EventWin_EndModal on the current window's owner's window
 * (+0x26) -- the dialog closes.
 *
 * RENAMED 2026-09-26 from owner_win_9fad: EventWin_EndModal on the DialogWin of the current window's owner --
 * the button handler Fame and HallOfFame install to close the dialog.
 *
 * RENAMED 2026-09-27 from end_owner_dialog: the Mac build's fame_OnLMouseDown, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: current_window then EventWin_EndModal on the owner: exactly the Mac fame_OnLMouseDown's two calls (GetEventWindow, EndModal); the button handler Fame installs, a callback whose (short, short) arguments go unread; ratio 0.84.
 */

#include "colonization.h"

#include "classes.h"
extern void *GetEventWindow(void);
extern void EventWin_EndModal(void *win);

void fame_OnLMouseDown(void)
{
    char *o;
    char *w = (char *)GetEventWindow();

    o = w != 0 ? w - 0x26 : w;
    EventWin_EndModal((EventWin *)(o + 0x26));
}
