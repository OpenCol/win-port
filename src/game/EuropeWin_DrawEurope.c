/* EuropeWin_DrawEurope: candidate for 1048:8cc9 (148 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawEurope
 * name-source: sibling-build
 *
 * Repaints the whole Europe screen: the 640x480 background, then with the window's nation made
 * current, the market, the status line, the two panels at 1048:91ac and 1048:990b, and the
 * docks, and finally copies all of it to the screen.
 *
 * Six of the calls are the 5-byte NOP-padded calls and are declared `extern`; the three
 * 4-byte `push cs` calls are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from europe_redraw: the Mac build's EuropeWin::DrawEurope(void): this only, repainting the whole Europe screen panel by panel.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cx, int cy);
extern void EuropeWin_DrawMarket(EuropeWin *w, int refresh);
extern void EuropeWin_DrawTitle(EuropeWin *w);
extern void EuropeWin_DrawEuropeShips(EuropeWin *w, int redraw);
extern void EuropeWin_DrawTransit(EuropeWin *w, int a);
extern void EuropeWin_DrawUnits(EuropeWin *w, int blit);

/* same object: 4-byte `push cs; call ` */
extern "C" void set_europe(int n);
extern "C" void compute_europe_parameters(void);

void EuropeWin_DrawEurope(EuropeWin *w)
{
    EuropeWin_UpdateEurope(w, 0, 0, 0x280, 0x1e0);
    set_europe(w->nation);
    compute_europe_parameters();
    EuropeWin_DrawMarket(w, 0);
    EuropeWin_DrawTitle(w);
    EuropeWin_DrawEuropeShips(w, 0);
    EuropeWin_DrawTransit(w, 0);
    EuropeWin_DrawUnits(w, 0);
    copy_all_to_screen_1048_dd24(w);
}
