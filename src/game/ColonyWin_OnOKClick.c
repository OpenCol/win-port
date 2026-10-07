/* PORT: this file's own declarations of ColonyWin_DoExit, EventWin_EndModal removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_OnOKClick: candidate for 1010:ae63 (115 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_OnOKClick
 * name-source: sibling-build
 *
 * EuropeWin_OnOKClick (1048:897a) with the end of the modal session made conditional on a
 * test of the object (ColonyWin_DoExit).
 *
 * RENAMED 2026-09-26 from refresh_current_window_port_if: the Mac build's ColonyWin::OnOKClick(short): ColonyWin_Init installs it as the action of the window's exit image button (+0x994) and on its region, and it ends the colony screen's modal session (EventWin_EndModal on the window) when the colony allows it. The old name misread the body: nothing is refreshed. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern void MSShowMenuBar(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColonyWin_OnOKClick(int unused1)
{
    ColonyWin *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (ColonyWin *)w;
    if (c == 0)
        return;
    MSShowMenuBar();
    if (ColonyWin_DoExit(c) != 0)
        EventWin_EndModal(&(DialogWin &)*c);
}
