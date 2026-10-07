/* PORT: this file's own declarations of ColChild_AddMyBucksToMessage, ColChild_BackspaceMessage removed: the header declares them from the definition (W1 step 3). */
/* EuropeWin_EuropeMarketMessage: candidate for 1048:1add (258 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMarketMessage
 * name-source: sibling-build
 *
 * The line that appears while a trade is being set up: "<nation> <buys|sells> <goods>
 * <for> <qty * price> <word> (<treasury>)". Both callers in this module build it the same
 * way -- set 0x4ce4/0x4ce6/0x4ce8, ask `europe_ask_price` or `europe_bid_price` for the unit
 * price, then come here -- and the 1 or 0 they pass as `buy` is the only difference between
 * the two, choosing word 13 over word 14.
 *
 * The parentheses are two one-character strings at DGROUP:0x2043 and 0x2045, with
 * `ColChild_BackspaceMessage` between them because `ColChild_AddMyBucksToMessage` leaves a
 * trailing space.
 *
 * ARGUMENT ORDER COMES FROM THE CALL SITES, and reading them needs care: the `PUSH 1` and
 * `PUSH [BP-4]` in the middle of the sequence are not both this function's. `europe_ask_price`
 * takes ONE argument and is cleaned with `ADD SP,2`; the 1 above it is already this
 * function's `buy`, pushed early because it is the fourth of five.
 *
 * `g_ctitle_word[CTITLE_GOLD]` (index 1) is a text-table INDEX appended straight after the
 * total, so by position it is a currency word; the code names both the table and the index
 * (labels.h), though nothing here reads the table.
 *
 * `set_europe` and `EuropeWin_EuropeActivateMessage` are -called THROUGH A NOP and
 * are external; the other seven have the four-byte form and are stubs (rule 47).
 *
 * RENAMED 2026-09-27 from trade_price_message: BEYOND THE PROPOSALS. Mac EuropeWin::EuropeMarketMessage(short x4), CODE 33 0x017c, 240 bytes, called by EuropeMouseMarket and EuropeMouseShipsCargo. 1add (EuropeWin *win, int goods, int price, int buy, int qty) is called by europe_market_click and europe_cargo_click (= those two). Callees: set_europe, CancelMessage, EuropeMessage x3, AddToMessage x2, AddNumberToMessage, BackspaceMessage, the bucks/string appends and EuropeActivateMessage (bsr) = msg_activate_redraw. Layout: Mac EuropeMessage, EuropeError, EuropeMarketMessage, EuropeClearBig; Windows EuropeWin_EuropeMessage, EuropeWin_EuropeError, 1add, EuropeWin_EuropeClearBig. Size 240 against 258. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "classes.h"

extern void set_europe(int n);
extern void EuropeWin_EuropeActivateMessage(EuropeWin *m, int kind, long ms);

/* same object: 4-byte `push cs; call ` */
extern "C" void ColChild_CancelMessage(EuropeWin *m, int a);
extern "C" void EuropeWin_EuropeMessage(EuropeWin *m, int i);
extern "C" void ColChild_AddToMessage(EuropeWin *m, int idx);

extern "C" void ColChild_AddStringToMessage(EuropeWin *m, char *s);

void EuropeWin_EuropeMarketMessage(EuropeWin *win, int goods, int price, int buy, int qty)
{
    set_europe(win->nation);
    ColChild_CancelMessage(win, 1);
    if (buy != 0)
        EuropeWin_EuropeMessage(win, 13);
    else
        EuropeWin_EuropeMessage(win, 14);
    ColChild_AddToMessage(win, g_goods_name_word[goods]);
    EuropeWin_EuropeMessage(win, 12);
    ColChild_AddNumberToMessage(win, qty * price);
    ColChild_AddToMessage(win, g_ctitle_word[CTITLE_GOLD]);
    ColChild_AddStringToMessage(win, "(");
    ColChild_AddMyBucksToMessage(win, g_cur_nation);
    ColChild_BackspaceMessage(win);
    ColChild_AddStringToMessage(win, ")");
    EuropeWin_EuropeActivateMessage(win, 1, 0L);
}
