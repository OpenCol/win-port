/* ReportWin_OnLaborMouseMove: candidate for 1058:31fd (18 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty hook of the labor report (ReportWin_ReportsLabor), left with a bare return.
 *
 * RENAMED 2026-09-26 from labor_report_hook: the Mac build's ReportWin::OnLaborMouseMove(short, short): ReportWin_ReportsLabor installs it as the labor report's mousemove handler. The Windows body is empty and reads no argument.
 */

#include "colonization.h"

void ReportWin_OnLaborMouseMove(void)
{
    return;
}
