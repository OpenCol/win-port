/* ColonyWin_ColonyMouseShipsCargo: candidate for 1010:52ff (335 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseShipsCargo
 * name-source: sibling-build
 *
 * A click on the cargo strip of the selected ship or wagon (DGROUP:0xc16):
 * slot (px - 252) / 24; its goods, slot and amount are noted at
 * 0x4baf..0x4bb1, the goods message shown, and the goods icon dragged
 * (ColChild_DragIcon); dropped on the warehouse panel (5) it unloads through
 * ColonyWin_AttemptCargoToColony, on panel 8 it goes to ColonyWin_ColonyMouseShipsProper
 * with the panel word (+0xa0a) held at 7.  ColonyMouseArea is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from colony_cargo_click: BEYOND THE PROPOSALS. Mac ColonyWin::ColonyMouseShipsCargo(short, short), CODE 13 0x20ec, 310 bytes. Callees: set_colony, nth_cargo_vessel, get_cargo, get_cargo_amount, ColonyMarketMessage, DragIcon, ColonyMouseArea x2, shift_key, AttemptCargoToColony and bsr ColonyMouseShipsProper. 52ff calls set_colony, nth_cargo_vessel, get_cargo, get_cargo_amount, ColonyWin_ColonyMarketMessage, ColChild_DragIcon, ColonyMouseArea x2, shift_key, ColonyWin_AttemptCargoToColony (the AttemptCargoToColony slot) and colony_dock_click. Caller: Mac (bsr) ColonyMouseParse, Windows colony_panel_rclick. Arity: this+2 against (w, px, py). Layout: between Market and Map on both. Size 310 against 335. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern int g_stub_a;
extern void set_colony(int colony);
extern int nth_cargo_vessel(int carrier);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void ColonyWin_ColonyMarketMessage(ColonyWin *w, int goods, int unused, int flag);
extern "C" void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y, int *rx, int *ry);   /* PORT: C linkage, as defined (W2) */
extern int shift_key(void);
extern "C" int ColonyWin_AttemptCargoToColony(void *w, int unit, int goods, int ask);   /* PORT: C linkage, as defined (W2) */
extern void ColonyWin_ColonyMouseShipsProper(ColonyWin *w, int x, int y);

extern "C" int ColonyMouseArea(int x, int y);

void ColonyWin_ColonyMouseShipsCargo(ColonyWin *w, int px, int py)
{
    register int u;
    int rx;
    int ry;
    int rel;
    int slot;
    int goods;

    set_colony(w->colony);
    rel = px - 0xfc;
    slot = rel / 24;
    u = nth_cargo_vessel(g_selected_carrier);
    goods = get_cargo(u, slot);
    if (goods >= 0) {
        g_drag_goods = goods;
        g_drag_slot = slot;
        g_drag_amount = get_cargo_amount(u, slot);
        ColonyWin_ColonyMarketMessage(w, goods, 0, 0);
        ColChild_DragIcon((ColChild *)w, &g_icon_sprites[ICON_GOODS + g_drag_goods], px, py, &rx, &ry);
        if (ColonyMouseArea(rx, ry) == 5) {
            ColonyWin_AttemptCargoToColony(w, u, g_drag_goods, shift_key());
            goto done;
        } else if (ColonyMouseArea(rx, ry) == 8) {
            w->panel = 7;
            ColonyWin_ColonyMouseShipsProper(w, rx, ry);
            w->panel = 0;
        }
    }
done:
    ;
}
