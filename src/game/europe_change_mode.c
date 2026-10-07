/* europe_change_mode: candidate for 1048:4e81 (76 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_change_mode
 * name-source: sibling-build
 *
 * Redraws the four panels of the Europe window in order -- the status line, the dock, the
 * market and the second dock list -- all on the object at SEG20:0x2fd2, the same one
 * `draw_europe_screen` (1048:4ecd, immediately after this) hands to `EuropeWin_DrawEurope`.
 * Only the status call takes the object alone; the other three take a trailing 1.
 *
 * Every call has a NOP before the `push cs`, the five-byte form TLINK leaves when it
 * shortens a call to an `extern`, so all four callees are declared and not defined
 * here (rule 47).
 *
 * RENAMED 2026-09-26 from europe_redraw_panels: the Mac build's europe_change_mode(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller EuropeKeyParse; caller EuropeMouseUnits; caller europe_click_ship; caller europe_recruit), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void EuropeWin_DrawTitle(EuropeWin *o);
extern void EuropeWin_DrawEuropeShips(EuropeWin *o, int redraw);
extern void EuropeWin_DrawMarket(EuropeWin *o, int refresh);
extern void EuropeWin_DrawUnits(EuropeWin *o, int blit);

void europe_change_mode(void)
{
    EuropeWin_DrawTitle(&g_europe_win);
    EuropeWin_DrawEuropeShips(&g_europe_win, 1);
    EuropeWin_DrawMarket(&g_europe_win, 1);
    EuropeWin_DrawUnits(&g_europe_win, 1);
}
