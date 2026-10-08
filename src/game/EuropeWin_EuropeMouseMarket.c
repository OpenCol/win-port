/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* EuropeWin_EuropeMouseMarket: candidate for 1048:2c69 (793 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseMarket
 * name-source: sibling-build
 * size-source: 793, not the census 784. The trailing 9 bytes are the dead closing-brace epilogue
 * after `done: return;`.
 *
 * Region 0 of the Europe screen, the market: sixteen goods columns 38 pixels wide from x 2. Like
 * the holds (`EuropeWin_EuropeMouseShipsCargo`) and the docks (`EuropeWin_EuropeMouseUnits`) it is a
 * click handler and a drop target, and returns unless the point is in its region -- and also
 * while a UNIT is being dragged (DGROUP:0x1fc4 at 8 or 9), which the market will not take.
 *
 * A GOODS DROP from a ship's hold is a sale, and the block that does it is the docks' block byte
 * for byte: the boycott check, then `EuropeWin_AttemptCargoToEurope`. Goods dragged out of the market and dropped
 * back on it do nothing.
 *
 * A CLICK on a column does one of three things:
 *
 *   - With DGROUP:0x4cf2 set -- `EuropeWin_MouseMove` sets it, the other two mouse handlers
 *     clear it -- a column other than the one at 0x1fd4 redraws the market and goes to
 *     `europe_label_cargo`, which records the column there.
 *   - Otherwise the column's goods become the selection (kind 0 at 0x1fc0, the goods at 0x1fc6,
 *     redrawn only if that changed), and in Colonopedia mode their article opens.
 *   - Otherwise a held button waits 15 ticks. A click that has been RELEASED by then on boycotted
 *     goods raises Parliament's notice; on goods that are not boycotted -- held or released --
 *     `EuropeWin_AttemptCargoFromEurope` is asked first, with its fourth argument 0, and only a 0 answer
 *     starts a BUY drag: source 1, 100 units, the buying price shown, and the drop offered to the
 *     market, the holds, region 1 and regions 2/3 in turn.
 *
 * A HELD BUTTON ON BOYCOTTED GOODS DOES NOTHING. The notice needs the button up and the drag needs
 * the goods unboycotted, so pressing and holding on a boycotted column falls between the two arms.
 *
 * The comparison of the selected goods is a byte compare, `(char)col` against the char at
 * 0x1fc6; the store back is a plain `g_market_col = col`, which Borland does through DL because AX
 * already holds the segment. The ending is the same `goto done` / `JMP SHORT +0` pair as the
 * holds (rule 62a).
 *
 * RENAMED 2026-09-27 from europe_market_click: BEYOND THE PROPOSALS; leads3 listed it as an unverified follow-up. Mac EuropeWin::EuropeMouseMarket(short, short), CODE 33 0x114a, 666 bytes. Callees: EuropeMouseArea, boycott x3, free_boycott x2, nth_europe_vessel x2, shift_key, AttemptCargoToEurope, set_europe, DrawMarket, europe_label_cargo, europe_change_mode, ShowModal, LButtonDownNow x2, MSWaitTicks, AttemptCargoFromEurope, europe_ask_price, EuropeMarketMessage, DragIcon, plus bsr to itself, ShipsCargo, ShipsProper and Transit. 2c69's callees are the same set (europe_sell_cargo, europe_buy_cargo and trade_price_message in those slots). Callers: Mac MouseMove, EuropeMouseParse, itself, ShipsCargo; Windows EuropeWin_MouseMove, dispatch_region_click, itself, europe_cargo_click. Arity: this+2 against (w, x, y). Layout: after AttemptEuropeShipToShip on both. Size 666 against 793. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e + 58 * (sizeof(void *) - 4)) ? 1 : -1];   /* PORT: +4 a pointer before it on 64-bit (W9) */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern char g_pedia_win;               /* SEG20:0x387c, declared */
#include "icons.h"

extern int EuropeMouseArea(int x, int y);
extern void set_europe(int n);
extern int boycott(int bit);
extern int free_boycott(int good);
extern int nth_europe_vessel(int n);
extern int shift_key(void);
extern int europe_ask_price(int g);
extern void EuropeWin_DrawMarket(EuropeWin *w, int flag);
extern "C" void europe_label_cargo(int goods);   /* PORT: C linkage, as defined (W2) */
extern "C" void europe_change_mode(void);   /* PORT: C linkage, as defined (W2) */
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern "C" void EuropeWin_EuropeMouseShipsCargo(EuropeWin *w, int x, int y);   /* PORT: C linkage, as defined (W2) */
extern void EuropeWin_EuropeMouseShipsProper(EuropeWin *w, int x, int y);
extern void EuropeWin_EuropeMouseTransit(EuropeWin *w, int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_DragIcon(EuropeWin *w, Sprite *sprite, int x, int y, int *rx, int *ry);
extern "C" void EuropeWin_EuropeMarketMessage(EuropeWin *win, int goods, int price, int buy, int qty);
extern "C" int EuropeWin_AttemptCargoFromEurope(EuropeWin *w, int u, int goods, int allowed, int ask);
extern "C" void EuropeWin_AttemptCargoToEurope(EuropeWin *w, int u, int goods, int ask);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void EuropeWin_EuropeMouseMarket(EuropeWin *w, register int x, register int y)
{
    int col;
    int ship;
    int rx;
    int ry;

    if (EuropeMouseArea(x, y) != 0)
        return;
    if (g_drag_mode == 8 || g_drag_mode == 9)
        return;
    if (g_drag_mode == 10) {
        if (g_goods_drag_from_market == 0) {
            if (boycott(g_selected_goods) != 0 && free_boycott(g_selected_goods) == 0)
                goto done;
            ship = nth_europe_vessel(g_europe_selected_ship);
            EuropeWin_AttemptCargoToEurope(w, ship, g_selected_goods, shift_key());
        }
    } else {
        set_europe(w->nation);
        col = (x - 2) / 0x26;
        if (col >= 0x10)
            goto done;
        if (g_europe_hover != 0) {
            if (g_europe_popup_id != col) {
                EuropeWin_DrawMarket(w, 0);
                europe_label_cargo(col);
            }
        } else {
            if (g_selection_kind != 0 || g_market_col != (char)col) {
                g_selection_kind = 0;
                g_market_col = col;
                europe_change_mode();
            }
            if (g_pedia_mode != 0) {
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 0, col, &(DialogWin &)*w);
            } else {
                if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0)
                    MSWaitTicks_1048_dbd6(0xf);
                if (Screen_LButtonDownNow(&(DialogWin &)*w) == 0 && boycott(col) != 0) {
                    free_boycott(col);
                } else if (boycott(col) == 0) {
                    if (EuropeWin_AttemptCargoFromEurope(w, nth_europe_vessel(g_europe_selected_ship), col, 0, 0) == 0) {
                        g_goods_drag_from_market = 1;
                        g_selected_goods = (unsigned char)col;
                        g_goods_drag_amount = 100;
                        EuropeWin_EuropeMarketMessage(w, col, europe_ask_price(col), 1, g_goods_drag_amount);
                        g_drag_mode = 10;
                        ColChild_DragIcon(w, &g_icon_sprites[ICON_GOODS + g_selected_goods], x, y, &rx, &ry);
                        EuropeWin_EuropeMouseMarket(w, rx, ry);
                        EuropeWin_EuropeMouseShipsCargo(w, rx, ry);
                        EuropeWin_EuropeMouseShipsProper(w, rx, ry);
                        EuropeWin_EuropeMouseTransit(w, rx, ry);
                        g_drag_mode = 0;
                    }
                    goto done;
                }
            }
        }
    }
done:
    return;
}
