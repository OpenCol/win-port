/* ReportWin_OnTradeRouteExtraClick: candidate for 1058:89ea (74 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnTradeRouteExtraClick
 * name-source: sibling-build
 *
 * Closes the report window that owns the current window (the object 0x26 bytes below it).
 *
 * RENAMED 2026-09-27 from on_trade_route_close_click: BEYOND THE PROPOSALS. Mac ReportWin::OnTradeRouteExtraClick(short), CODE 18 0x80c2, the last function of the segment. Its seq is GetEventWindow, ReportWin::Done. 89ea (void), 74 bytes, calls GetEventWindow and ReportWin_Done. Layout: directly after OnTradeRouteClick on both. Arity: a button handler, (void) against (short), as for OnColonyPrevClick. ReportWin_Done is then probably ReportWin::Done (not verified; see chains).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ReportWin_OnTradeRouteExtraClick(void)
{
    ReportWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ReportWin *)w;

    ReportWin_Done(o);
}
