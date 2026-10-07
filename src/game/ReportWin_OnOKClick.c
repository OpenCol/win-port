/* ReportWin_OnOKClick: candidate for 1058:0bde (88 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnOKClick
 *
 * ReportWin_Done on the object owning the current window; nothing when there is none.
 *
 * RENAMED 2026-09-27 from ReportWin_Done: WRONG EXISTING NAME, CHAIN 2 step 1 (vacates ReportWin_Done). This is Mac ReportWin::OnOKClick(short), not ReportWin::Done. Install site: Mac ReportWin::Init(char, EventWin*) takes OnOKClick's address (lea at +0x490) for its OK button. Windows ReportWin_Init (1058:0021) pushes this function (dw @0x01ed) to set_control_action_1058_bb68 for the image button it just built. Layout: Mac KeyClick, Close, OnOKClick, LeftMouseClick, ReportsIndian; Windows ReportWin_KeyClick, ReportWin_Close, 0bde, 0c36, ReportsIndian. Body: Mac OnOKClick is GetEventWindow + Done (66 bytes); 0bde is GetEventWindow + close_report_window (88 bytes). check_findings' comment says finding 58 bound Done here through the slot families and that OnOKClick 'has no counterpart in this build at all'; the install site and the layout show the counterpart is this function. The (void) spelling is the Windows handler convention (no this; the id argument unread).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ReportWin_OnOKClick(void)
{
    ReportWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ReportWin *)w;
    if (o == 0)
        return;
    ReportWin_Done(o);
}
