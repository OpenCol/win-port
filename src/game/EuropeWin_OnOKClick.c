/* PORT: this file's own declarations of EventWin_EndModal removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_OnOKClick: candidate for 1048:897a (98 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_OnOKClick
 * name-source: sibling-build
 *
 * Steps back 0x26 from the current window's port to its object, and when there is one,
 * ends its modal session (EventWin_EndModal) after MSShowMenuBar (empty in this build) -- the screen closes.
 *
 * RENAMED 2026-09-26 from refresh_current_window_port: the Mac build's EuropeWin::OnOKClick(short): EuropeWin_Init installs it as the action of the window's image button and on its region, and it ends the Europe screen's modal session (EventWin_EndModal on the window). The old name misread the body: nothing is refreshed. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern void MSShowMenuBar(void);

void EuropeWin_OnOKClick(int unused1)
{
    EuropeWin *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (EuropeWin *)w;

    if (c == 0)
        return;
    MSShowMenuBar();
    EventWin_EndModal(&(EventWin &)*c);
}
