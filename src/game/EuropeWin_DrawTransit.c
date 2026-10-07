/* EuropeWin_DrawTransit: candidate for 1048:990b (47 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawTransit
 * name-source: sibling-build
 *
 * Redraws two panels of the Europe window with the same refresh flag: the dock through
 * `EuropeWin_DrawInbound`, then the left panel through `EuropeWin_DrawOutBound`.
 *
 * `refresh` is held in SI from entry (`push si; mov si,[bp+0xa]`), because it is used twice;
 * under -Od only a `register` declaration puts a parameter there. Both calls are the
 * four-byte `push cs; call `, so both callees are defined here (rule 47).
 *
 * RENAMED 2026-09-26 from europe_draw_dock_left: the Mac build's EuropeWin::DrawTransit(short): this plus a refresh flag, redrawing the two sailing panels (EuropeWin_DrawInbound and EuropeWin_DrawOutBound) together. Which of those two is DrawInbound and which DrawOutBound is not settled, so they keep descriptive names.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void EuropeWin_DrawInbound(EuropeWin *w, int refresh);
extern "C" void EuropeWin_DrawOutBound(EuropeWin *w, int refresh);

void EuropeWin_DrawTransit(EuropeWin *w, register int refresh)
{
    EuropeWin_DrawInbound(w, refresh);
    EuropeWin_DrawOutBound(w, refresh);
}
