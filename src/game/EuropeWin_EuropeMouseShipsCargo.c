/* EuropeWin_EuropeMouseShipsCargo: candidate for 1048:2f82 (539 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseShipsCargo
 * name-source: sibling-build
 * size-source: 539, not the census 530. The trailing 9 bytes are the dead closing-brace epilogue
 * after `done: return;`.
 *
 * A click in the in-port ship's cargo holds on the Europe screen -- and the drop target for goods
 * dragged there. It returns at once unless the point is in region 10, which is what lets one
 * function be both: `EuropeWin_EuropeMouseParse` sends it clicks, and a goods drag offers its drop
 * point to each of the four goods regions in turn, this one included. The Europe screen's
 * sibling of `ColonyWin_ColonyMouseShipsCargo`.
 *
 * With no ship in port (DGROUP:0x1fca) nothing happens. Otherwise the hold is
 * `(x - 0x123) / 24`, the ship is the port chain's entry at 0x4cde, and one of three things:
 *
 *   - A goods drag is in progress (0x1fc4 == 10), so this is a DROP. Goods that came from the
 *     market (`EuropeWin_EuropeMouseMarket` sets source 0x4ce4 to 1) are loaded through
 *     `EuropeWin_AttemptCargoFromEurope`; goods that came from a hold do nothing.
 *   - Colonopedia mode (0x9dc): a loaded hold opens its goods' article.
 *   - The button is still held on a loaded hold: a drag starts. Source 0, the goods, hold and
 *     amount go to 0x4ce6/0x4ce0/0x4ce8, the selling price is shown, 0x1fc4 is 10 for the
 *     duration, and `ColChild_DragIcon` drags the goods icon. The drop point is then offered to
 *     the market, to THIS FUNCTION, to region 1 and to regions 2/3, and the flag is cleared.
 *
 * THE RECURSION IS HOW A HOLD REFUSES ITS OWN GOODS. The nested call sees a drag in progress
 * from source 0 and does nothing, so cargo dropped back onto the holds -- on its own hold or
 * another -- is neither moved nor started again.
 *
 * All three stored values are narrowed to a byte on the way into word globals -- the goods,
 * the hold, and `get_cargo_amount`'s int result, a `MOV AH,0` each (rule 51).
 *
 * Two `goto done`s. The no-ship test is the two-jump form (rule 12); the one after the drag
 * arm is the `JMP SHORT +0` of rule 62a -- the loaded-hold test lands ON it and the button
 * test lands after it, which an empty `else` would reverse.
 *
 * THE GOODS-DRAG GROUP at DGROUP:0x4ce0..0x4ce8 is one object with four words, set when a
 * goods drag starts and read when it is dropped:
 *
 *   0x4ce0  g_goods_drag_slot         the hold it came out of (ship drags only)
 *   0x4ce4  g_goods_drag_from_market  1 from the market shelf, 0 from a ship's hold
 *   0x4ce6  g_selected_goods          which goods
 *   0x4ce8  g_goods_drag_amount       how much -- the hold's contents, or 100 from the market
 *
 * `g_goods_drag_from_market` is what every drop site branches on while `g_drag_mode` is 10.
 * Dropped on the market or the dock it SELLS when the goods came from a hold; dropped on
 * the cargo strip it BUYS when they came from the shelf; dropped on another ship it is a
 * transfer one way and a purchase the other.
 *
 * RENAMED 2026-09-27 from europe_cargo_click: BEYOND THE PROPOSALS; a leads3 follow-up. Mac EuropeWin::EuropeMouseShipsCargo(short, short), CODE 33 0x13e4, 478 bytes. Callees: EuropeMouseArea, set_europe, nth_europe_vessel, get_cargo, shift_key, AttemptCargoFromEurope, ShowModal, LButtonDownNow, get_cargo_amount, europe_bid_price, EuropeMarketMessage, DragIcon, EuropeMouseMarket, plus bsr to itself, ShipsProper and Transit. 2f82 calls exactly these (europe_buy_cargo, PediaWin_ShowModal, trade_price_message, europe_market_click, europe_ships_click, europe_voyage_click). Callers: Mac EuropeMouseParse, EuropeMouseMarket, itself, ShipsProper; Windows dispatch_region_click, europe_market_click, itself, europe_ships_click. Arity: this+2 against (w, x, y). Layout: between Market and ShipsProper on both. Size 478 against 539. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
extern int g_ships_in_port;               /* DGROUP:0x1fca, declared */

extern char g_pedia_win;               /* SEG20:0x387c, declared */
#include "icons.h"

extern int EuropeMouseArea(int x, int y);
extern void set_europe(int n);
extern int nth_europe_vessel(int n);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern int shift_key(void);
extern int europe_bid_price(int g);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern void EuropeWin_EuropeMouseShipsProper(EuropeWin *w, int x, int y);
extern void EuropeWin_EuropeMouseTransit(EuropeWin *w, int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_DragIcon(EuropeWin *w, Sprite *sprite, int x, int y, int *rx, int *ry);
extern "C" void EuropeWin_EuropeMarketMessage(EuropeWin *win, int goods, int price, int buy, int qty);
extern "C" int EuropeWin_AttemptCargoFromEurope(EuropeWin *w, int u, int goods, int allowed, int ask);
extern "C" void EuropeWin_EuropeMouseMarket(EuropeWin *w, int x, int y);

void EuropeWin_EuropeMouseShipsCargo(EuropeWin *w, register int x, int y)
{
    int rel;
    int slot;
    int goods;
    int rx;
    int ry;
    int u;

    if (EuropeMouseArea(x, y) != 10)
        return;
    set_europe(w->nation);
    if (g_ships_in_port == 0)
        goto done;
    rel = x - 0x123;
    slot = rel / 24;
    u = nth_europe_vessel(g_europe_selected_ship);
    goods = get_cargo(u, slot);
    if (g_drag_mode == 10) {
        if (g_goods_drag_from_market == 1)
            EuropeWin_AttemptCargoFromEurope(w, u, g_selected_goods, 1, shift_key());
    } else if (g_pedia_mode != 0) {
        if (goods >= 0)
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 0, goods, &(DialogWin &)*w);
    } else if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0) {
        if (goods >= 0) {
            g_goods_drag_from_market = 0;
            g_selected_goods = (unsigned char)goods;
            g_goods_drag_slot = (unsigned char)slot;
            g_goods_drag_amount = (unsigned char)get_cargo_amount(u, slot);
            EuropeWin_EuropeMarketMessage(w, goods, europe_bid_price(goods), 0, g_goods_drag_amount);
            g_drag_mode = 10;
            ColChild_DragIcon(w, &g_icon_sprites[ICON_GOODS + goods], x, y, &rx, &ry);
            EuropeWin_EuropeMouseMarket(w, rx, ry);
            EuropeWin_EuropeMouseShipsCargo(w, rx, ry);
            EuropeWin_EuropeMouseShipsProper(w, rx, ry);
            EuropeWin_EuropeMouseTransit(w, rx, ry);
            g_drag_mode = 0;
        }
        goto done;
    }
done:
    return;
}
