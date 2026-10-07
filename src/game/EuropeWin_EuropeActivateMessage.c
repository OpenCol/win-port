/* PORT: this file's own declarations of ColChild_DrawAnyMessage removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_EuropeActivateMessage: candidate for 1048:1c79 (68 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeActivateMessage
 * name-source: sibling-build
 *
 * Makes the window's own nation current, activates a message of the given kind, then redraws the message area.
 *
 * RENAMED 2026-09-27 from msg_activate_redraw: BEYOND THE PROPOSALS. Mac EuropeWin::EuropeActivateMessage(short, long), CODE 33 0x036c, 100 bytes. Its seq is set_europe, ColChild::ActivateMessage, DrawAnyMessage, and 1c79 calls set_europe, ColChild_ActivateMessage and ColChild_DrawAnyMessage. Callers, six for six: Mac AttemptCargoFromEurope, AttemptCargoToEurope, AttemptEuropeShipToShip, Show, and bsr EuropeError, EuropeMarketMessage; Windows europe_buy_cargo, europe_sell_cargo, europe_transfer_cargo, EuropeWin_Show, EuropeWin_EuropeError, trade_price_message. Arity: this+(short, long) against (ColChild *win, char kind, long ms). Layout: after EuropeShowBig, before AttemptCargoFromEurope, on both. Size 100 against 68. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void set_europe(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_ActivateMessage(ColChild *child, int kind, long arg);
void EuropeWin_EuropeActivateMessage(EuropeWin *win, int kind, long ms)
{
    set_europe(win->nation);
    ColChild_ActivateMessage(win, kind, ms);
    ColChild_DrawAnyMessage(win, 1, 0L);
}
