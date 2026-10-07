/* PORT: this file's own declarations of ColChild_IsSmallDesktop removed: the header declares it from the definition (W1 step 3). */
/* ReportWin_LeftMouseClick: candidate for 1058:0c36 (117 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_LeftMouseClick
 * name-source: sibling-build
 *
 * Closes the current report window when the desktop is narrow or the window's flag at
 * +0x7f8 is set. ColChild_IsSmallDesktop receives the window though it ignores it.
 *
 * RENAMED 2026-09-27 from ReportWin_LeftMouseClick: the Mac build's dismiss_report_on_click, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: current_window, then ColChild_IsSmallDesktop, then close the report: the same three calls as Mac ReportWin::LeftMouseClick (GetEventWindow, IsSmallDesktop, Done); a set_on_* handler with (short, short) and no this; ratio 0.79.
 *
 * RENAMED 2026-09-27 from ReportWin_LeftMouseClick: reverted the same day: finding 59 withdrew ReportWin::LeftMouseClick for this function -- the complete install table puts the LeftMouseClick slot at 0 and this function on slots 12 and 24 -- and the alignment vote does not outweigh the install table.
 *
 * RENAMED 2026-09-27 from dismiss_report_on_click: CHAIN 2 step 3. Mac ReportWin::LeftMouseClick(short, short). FINDING 59'S WITHDRAWAL IS REFUTED by the Mac build's own install table. Mac ReportWin::ReallyDrawReport stores LeftMouseClick's address into the handler block at base 0x40 + 0x0c and 0x40 + 0x18 (lea $e30 twice, move.l a0,$c(a2,d0.l) and $18(a2,d0.l) with d0 = 0x40). Windows ReportWin_ReallyDrawReport installs 0c36 on slots 12 and 24 (re-slotmap.json). These are the same two slots. Finding 59 argued that slot 0 is 'the LeftMouseClick slot'; the Mac shows that ReportWin's LeftMouseClick is registered on 12/24. The BSR edge adds a direct caller: Mac ReportWin::OnLaborClick calls LeftMouseClick, and Windows ReportWin_OnLaborClick -calls 0c36. Callees: GetEventWindow, IsSmallDesktop, Done (close_report_window) on both. Layout: after OnOKClick (0bde), before ReportsIndian, on both. Width: 2 shorts; the Windows handler drops this, as every handler in 1058 does. Size 117/92. FOLLOW-UP: check_findings.py's '_groups = {OnOKClick, LeftMouseClick}' check, the 'withdrawn ReportWin mouse handler sits on 12 and 24, not 0' check and the finding 58/59 prose all need rewriting. RE_STATUS's ReportWin bound count rises by two.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */


void ReportWin_LeftMouseClick(int unused1, int unused2)
{
    ReportWin *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (ReportWin *)w;
    if (c == 0)
        return;
    if (ColChild_IsSmallDesktop(c) != 0 || c->modal != 0)
        ReportWin_Done(c);
}
