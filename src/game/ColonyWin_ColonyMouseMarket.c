/* PORT: this file's own declarations of label_cargo removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_ColonyMouseMarket: candidate for 1010:510b (500 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseMarket
 * name-source: sibling-build
 *
 * Picks up a pile of goods from the colony's warehouse strip and carries it. The x the click
 * landed on divides by 0x26 into a goods index -- sixteen slots -- and anything past fifteen
 * is not a pile and does nothing. With the drag machinery already armed (DGROUP:0x9ea) the
 * click only re-aims it.
 *
 * Otherwise the pile is picked up: DGROUP:0x4bae says a drag is live, 0x4baf holds the goods,
 * and 0x4bb0 holds min(colony stock, 100) -- the hundred is the cap on one load, so a
 * warehouse with more than that hands over a full hundred and keeps the rest.
 *
 * The cursor sprite says which: `g_drag_amount >= 0x64 ? 0x17 : 0x27` picks between a full
 * pile and a part one, and the goods index is added to whichever base. It is a TERNARY -- both
 * arms load AX and the store happens once at the join, where an if/else stores in each arm.
 *
 * `ColChild_DragIcon` runs the drag and reports where it ended. Panel 7 is the cargo strip and
 * the pile goes into the hold; panel 8 is the ship strip, which first SELECTS the ship under
 * the pointer through `ColonyWin_ColonyMouseShipsProper` -- with the window's panel word forced to 6
 * so the click reads as a selection -- and then loads it.
 *
 * THE `else` BLOCK ENDS WITH A REDUNDANT `goto out`, and that is what the two-byte `EB 00`
 * before the epilogue is. It is not rule 54's empty `else`: with an empty `else` the tests land
 * AFTER the jump, and here the `JNZ NEAR` from the prompt's test lands ON it. The jump is the
 * `if` body's end, which is only true when a `goto` to the next line is the statement after it.
 *
 * RENAMED 2026-09-27 from colony_goods_click: CONST PROPOSAL. Mac ColonyWin::ColonyMouseMarket(short, short), CODE 13 0x1f0e, 478 bytes. Callees: set_colony, nth_cargo_vessel x2, label_cargo, DrawMarket, AttemptCargoToShip x3, ColonyMarketMessage, DragIcon, ColonyMouseArea x2, shift_key x2, and bsr ColonyMouseShipsProper. 510b calls set_colony, nth_cargo_vessel x2, label_cargo, ColonyWin_DrawMarket, ColonyWin_AttemptCargoToShip x3 (the AttemptCargoToShip slot), ColonyWin_ColonyMarketMessage, ColChild_DragIcon, ColonyMouseArea x2, shift_key and colony_dock_click (= ShipsProper, below). Callers: Mac (bsr) ColonyMouseParse and ColonyMouseMove; Windows colony_panel_rclick and colony_panel_click (= those two, above). Arity: this+(short, short) against (ColonyWin *w, int x, int y). Layout: third after KeyClick on both. Size 478 against 500. Constants: overlap 0.80, runner-up 0.40. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern void set_colony(int colony);
extern int nth_cargo_vessel(int carrier);
extern void ColonyWin_DrawMarket(ColonyWin *w, int redraw);
extern int ColonyWin_AttemptCargoToShip(ColonyWin *w, int u, int goods, int a, int b);
extern void ColonyWin_ColonyMarketMessage(ColonyWin *w, int goods, int a, int flag);
extern void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y,
                                  int *mx, int *my);
extern int shift_key(void);
extern void ColonyWin_ColonyMouseShipsProper(ColonyWin *w, int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int ColonyMouseArea(int x, int y);

void ColonyWin_ColonyMouseMarket(ColonyWin *w, int x, int y)
{
    int goods;
    int mx;
    int my;
    int base;
    register int u;

    set_colony(w->colony);
    goods = (x - 2) / 0x26;
    u = nth_cargo_vessel(g_selected_carrier);
    g_c12 = goods;
    if (goods >= 0x10)
        goto out;
    if (g_hover_mode != 0) {
        if (g_labelled_goods != goods)
            label_cargo(goods);
    } else {
        ColonyWin_DrawMarket(w, 1);
        if (ColonyWin_AttemptCargoToShip(w, u, goods, 0, 0) == 0) {
            g_drag_active = 1;
            g_drag_goods = goods;
            g_drag_amount = min_int_1010_e5d0(g_cur_colony_rec->stock[goods], 0x64);
            ColonyWin_ColonyMarketMessage(w, goods, 0, 1);
            base = g_drag_amount >= 0x64 ? 0x17 : 0x27;
            ColChild_DragIcon((ColChild *)w, &g_icon_sprites[g_drag_goods + base], x, y, &mx, &my);
            if (ColonyMouseArea(mx, my) == 7) {
                ColonyWin_AttemptCargoToShip(w, u, g_drag_goods, 1, shift_key());
            } else if (ColonyMouseArea(mx, my) == 8) {
                w->panel = 6;
                ColonyWin_ColonyMouseShipsProper(w, mx, my);
                w->panel = 0;
                u = nth_cargo_vessel(g_selected_carrier);
                ColonyWin_AttemptCargoToShip(w, u, g_drag_goods, 1, shift_key());
            }
        }
        goto out;
    }
out:
    ;
}
