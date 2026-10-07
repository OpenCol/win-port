/* ReportWin_OnLaborClick: candidate for 1058:3b14 (247 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnLaborClick
 * name-source: sibling-build
 *
 * A click in the labour report: each of the 29 specialty cells (columns at +0x7fe, rows at
 * +0x838, 206 wide and 37 high) is hit-tested; a hit opens the colonist detail for that
 * cell, a miss dismisses the report.
 *
 * RENAMED 2026-09-26 from on_labor_report_click: the Mac build's ReportWin::OnLaborClick(short, short): ReportWin_ReportsLabor installs it as the labor report's left-click handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ReportWin_LeftMouseClick(int x, int y);
extern "C" void ReportWin_DoExtendedLaborReport(ReportWin *o, int x, int y);
void ReportWin_OnLaborClick(int x, int y)
{
    ReportWin *o;
    int hit;
    RECT r;
    EventWin *w = (EventWin *)GetEventWindow();
    int i;

    o = (ReportWin *)w;
    if (o == 0)
        return;
    hit = -1;
    for (i = 0; i < 0x1d; i++) {
        SetRect(&r, o->cell_x[i], o->cell_y[i] - 1, o->cell_x[i] + 0xce, o->cell_y[i] + 0x24);
        if (x > r.left && x < r.right && y > r.top && y < r.bottom)
            hit = i;
    }
    if (hit < 0) {
        ReportWin_LeftMouseClick(x, y);
        return;
    }
    ReportWin_DoExtendedLaborReport(o, x, y);
}
