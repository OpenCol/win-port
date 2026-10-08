/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* EuropeWin_EuropeMouseShipsProper: candidate for 1048:319d (866 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseShipsProper
 * name-source: sibling-build
 * size-source: 866, not the census 857. The trailing 9 bytes are the dead closing-brace epilogue
 * after `done: return;`.
 *
 * Region 1 of the Europe screen: the ships in port. The fourth Europe drop target beside the
 * market (`EuropeWin_EuropeMouseMarket`), the holds (`EuropeWin_EuropeMouseShipsCargo`) and the
 * docks (`EuropeWin_EuropeMouseUnits`), and the one that starts the SHIP drag. It returns unless
 * the point is in region 1, then finds the ship under it by laying out each of the DGROUP:0x1fca
 * in-port entries with `get_transit_loc` and testing the cell; nothing hit, nothing done.
 *
 * A GOODS DROP on a ship: from a hold, `EuropeWin_AttemptEuropeShip2Ship` moves the cargo from the selected ship's
 * hold (0x4ce0) into the ship under the drop -- a transfer, so no boycott check; from the market,
 * `EuropeWin_AttemptCargoFromEurope` buys it straight into that ship.
 *
 * A CLICK, after the usual 15-tick wait on a held button:
 *
 *   - Released: a ship that is not the selected one (the pair at 0x4cde/0x4ce2) becomes it and the
 *     screen is redrawn; the ship that already is opens `europe_click_ship`'s menu. In Colonopedia
 *     mode the ship type's article opens instead.
 *   - Still held: the ship is selected as above (kind 1 at 0x1fc0, the previous selection kept at
 *     0x1fbe), and dragged as its portrait with 0x1fc4 at 8. The drop is offered to the holds, to
 *     THIS FUNCTION, and to regions 2/3 -- not to the market, which refuses units, nor the docks.
 *
 * A ship with bit 0x80 of Unit+4 set is not dragged -- unless its type is 0xb, Artillery, which no
 * ship is. The exemption can never apply here; it reads like a test carried over from code that
 * drags land units.
 *
 * THE DROP COMES BACK AS A CLICK. The nested call runs with the button released, so it takes the
 * click arm: a ship dropped on another ship selects that one, and a ship dropped back on itself
 * opens its own menu. The `0x1fc4 == 8` test after the click arm only matters to a nested call
 * made with the button still down, where it stops a second drag starting inside the first.
 *
 * The selected-ship test has no `else`. Its arm ends in `goto done` and the menu call simply
 * follows; written with an `else`, a dead jump over the menu call would sit after that `goto`,
 * and the shipped code has none there. The one dead jump it does have closes the non-pedia arm.
 *
 * `g_1fbe` IS WRITTEN ONCE AND NEVER READ. This store of the selected ship is the only
 * instruction in the image touching 0x1fbe (`tools/check_dead_dgroup.py`), and DGROUP ships the
 * cell as 0xffff -- an initialised "none" for a remembered selection that nothing remembers.
 * The name would have to be `g_last_selected_ship`, which claims a reader; the address stays.
 *
 * RENAMED 2026-09-27 from europe_ships_click: CONST PROPOSAL. Mac EuropeWin::EuropeMouseShipsProper(short, short), CODE 33 0x15c2, 794 bytes. Callees: EuropeMouseArea, set_europe, get_transit_loc, InBox, nth_europe_vessel x8, shift_key x2, AttemptEuropeShipToShip, AttemptCargoFromEurope, LButtonDownNow x3, MSWaitTicks, europe_change_mode x2, europe_click_ship, ShowModal, UnitIcon, DragIcon, EuropeMouseShipsCargo, plus bsr to itself and Transit. 319d calls the same (classify_index_into_tier = get_transit_loc, europe_transfer_cargo, europe_buy_cargo, nth_europe_vessel x8, Screen_LButtonDownNow x3, ...). Callers: Mac EuropeMouseParse, Market, ShipsCargo, itself, Transit; Windows dispatch_region_click, europe_market_click, europe_cargo_click, itself, europe_voyage_click. Arity: this+2 against (EuropeWin *w, int x, int y). Layout: after ShipsCargo, before Transit, on both. Size 794 against 866. Constants: overlap 0.73. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "units.h"
#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e) ? 1 : -1];
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "records.h"

extern int g_ships_in_port;               /* DGROUP:0x1fca, declared */

extern char g_pedia_win;               /* SEG20:0x387c, declared */
extern int EuropeMouseArea(int x, int y);
extern void set_europe(int n);
extern void get_transit_loc(int idx, int base_x, int tier3_count, int extra,
                                         int *tier, int *x, int *sprite,
                                         int *w, int *size);
extern int InBox(int x, int y, int rx, int ry, int rw, int rh);
extern int nth_europe_vessel(int n);
extern int shift_key(void);
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern "C" void europe_change_mode(void);   /* PORT: C linkage, as defined (W2) */
extern void europe_click_ship(int unit, int in_port);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
extern void EuropeWin_EuropeMouseTransit(EuropeWin *w, int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_DragIcon(EuropeWin *w, Sprite *sprite, int x, int y, int *rx, int *ry);
extern "C" int EuropeWin_AttemptCargoFromEurope(EuropeWin *w, int u, int goods, int allowed, int ask);
extern "C" void EuropeWin_AttemptEuropeShip2Ship(EuropeWin *w, int from, int to, int slot, int ask);
extern "C" void EuropeWin_EuropeMouseShipsCargo(EuropeWin *w, int x, int y);

void EuropeWin_EuropeMouseShipsProper(EuropeWin *w, int x, int y)
{
    int ship;
    int to_ship;
    int tier;
    int cx;
    int cy;
    int cw;
    int ch;
    int rx;
    int ry;
    register int found;
    register int i;

    if (EuropeMouseArea(x, y) != 1)
        return;
    set_europe(w->nation);
    for (found = -1, i = 0; found < 0 && g_ships_in_port > i; i++) {
        get_transit_loc(i, 0x11f, 5, 2, &tier, &cx, &cy, &cw, &ch);
        if (InBox(x, y, cx, cy, cw, ch) != 0)
            found = i;
    }
    if (found < 0)
        goto done;
    if (g_drag_mode == 10) {
        if (g_goods_drag_from_market == 0) {
            ship = nth_europe_vessel(g_europe_selected_ship);
            to_ship = nth_europe_vessel(found);
            EuropeWin_AttemptEuropeShip2Ship(w, ship, to_ship, g_goods_drag_slot, shift_key());
        } else {
            ship = nth_europe_vessel(found);
            EuropeWin_AttemptCargoFromEurope(w, ship, g_selected_goods, 1, shift_key());
        }
    } else {
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0)
            MSWaitTicks_1048_dbd6(0xf);
        if (Screen_LButtonDownNow(&(DialogWin &)*w) == 0) {
            if (g_pedia_mode == 0) {
                if (g_europe_selected_ship != found || g_europe_ship_highlight != found) {
                    g_europe_selected_ship = g_europe_ship_highlight = found;
                    europe_change_mode();
                    goto done;
                }
                europe_click_ship(nth_europe_vessel(found), 0);
                goto done;
            } else if (g_ships_in_port != 0) {
                found = nth_europe_vessel(found);
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, g_units[found].type, &(DialogWin &)*w);
                goto done;
            }
        }
        if (g_drag_mode == 8)
            goto done;
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0) {
            g_1fbe = g_europe_selected_ship;
            g_selection_kind = 1;
            if (g_europe_selected_ship != found || g_europe_ship_highlight != found) {
                g_europe_selected_ship = g_europe_ship_highlight = found;
                europe_change_mode();
            }
            if ((g_units[nth_europe_vessel(found)].flags2 & 0x80) != 0
             && g_units[nth_europe_vessel(found)].type != UNIT_ARTILLERY)
                goto done;
            g_drag_mode = 8;
            ColChild_DragIcon(w, &g_icon_sprites[UnitIcon(nth_europe_vessel(g_europe_selected_ship))], x, y, &rx, &ry);
            EuropeWin_EuropeMouseShipsCargo(w, rx, ry);
            EuropeWin_EuropeMouseShipsProper(w, rx, ry);
            EuropeWin_EuropeMouseTransit(w, rx, ry);
            g_drag_mode = 0;
        }
    }
done:
    return;
}
