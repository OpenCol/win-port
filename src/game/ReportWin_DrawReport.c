/* ReportWin_DrawReport: candidate for 1058:06d0 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_DrawReport
 * name-source: sibling-build
 *
 * The report window's repaint handler (installed by ReportWin_Init): draws the current
 * window's owner through ReportWin_ReallyDrawReport (a call TLINK converted).
 *
 * RENAMED 2026-09-26 from repaint_report_window: the Mac build's ReportWin::DrawReport(void): the entry -- ReportWin_Init installs it as the repaint handler, and all it does is hand the window to ReportWin_ReallyDrawReport. (The Windows handler takes no `this`; it finds the window itself.) The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
extern void ReportWin_ReallyDrawReport(ReportWin *o);

void ReportWin_DrawReport(void)
{
    ReportWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ReportWin *)w;
    ReportWin_ReallyDrawReport(o);
}
