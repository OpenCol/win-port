/* ColonyWin_ColonyMouseShipsProper: candidate for 1010:6b24 (505 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseShipsProper
 * name-source: sibling-build
 * size-source: 505, not the census 503 -- the census stops two bytes inside the epilogue.
 *
 * A click on the colony window's ship strip. The x it lands on becomes a slot, and WHICH
 * STRIP decides the arithmetic: at or below y = 0x132 the wide row is `(minimax(x - 0x104, 0,
 * 0x83) + 0x10) / 0x22`, four sprites 0x22 apart; above it (y < 0x132) the narrow row is
 * `(minimax(x - 0xf5, 0, 0x20e) + 1) / 5 + 4`, and the +4 is what puts the narrow row's
 * slots after the wide row's. Either way the slot is clamped to the carrier count.
 *
 * What happens then is the window's panel word (+0xa0a). Panel 6 selects the ship -- unless
 * it is already selected, in which case nothing at all happens. Panel 7 is the cargo strip
 * and drags the held slot from the selected carrier to the one clicked. Any other panel is
 * the plain click: it records the selection for the drag machinery, and then either opens
 * the ship (a second click on the one already chosen), selects it, or -- with DGROUP:0x9dc
 * set, which is the Colonopedia cursor -- opens the pedia on the ship's type instead.
 *
 * `g_selected_carrier = g_carrier_highlight = slot` is one chained assignment in both places it appears:
 * 0xc18 is stored first and the value survives in a register for 0xc16 (`MOV AX,DX`).
 *
 * THE UNIT INDEX REUSES `slot`. The shipped puts `nth_cargo_vessel`'s result in SI,
 * which is the slot's register; a separate local lands in DI and is a byte out.
 *
 * `slot` is the register local and `n` a plain local Borland promotes -- spelling `n`
 * `register` gives it SI and moves the slot to DI (rule 53).
 *
 * RENAMED 2026-09-27 from colony_dock_click: CONST PROPOSAL. Mac ColonyWin::ColonyMouseShipsProper(short, short), CODE 13 0x3806, 540 bytes. Its seq is minimax x2, colony_change_mode, nth_cargo_vessel x2, shift_key, AttemptShipToShip, colony_click_ship, colony_change_mode, nth_cargo_vessel, ShowModal, reset_colony. 6b24 calls minimax x2, colony_change_mode x2, nth_cargo_vessel x3, shift_key, 1010:b882 (= ColonyWin_AttemptShipToShip, leads3), colony_click_ship, PediaWin_ShowModal and reset_colony. Callers: Mac (bsr) ColonyMouseParse, ColonyMouseMarket, ColonyMouseShipsCargo; Windows colony_panel_rclick, colony_goods_click, colony_cargo_click, the same three. Arity: this+2 against (w, x, y). Layout: directly after colony_click_ship on both. Size 540 against 505. Constants: overlap 0.89, rare 0x20e. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









#include "records.h"

extern char g_pedia_win;              /* SEG20:0x387c, declared */

extern int nth_cargo_vessel(int carrier);
extern int shift_key(void);
extern void ColonyWin_AttemptShipToShip(ColonyWin *w, int from, int to, int slot, int ask);
extern void PediaWin_ShowModal(void *ctx, int category, int type, void *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void colony_change_mode(void);

extern "C" void reset_colony(void);

void ColonyWin_ColonyMouseShipsProper(ColonyWin *w, int x, int y)
{
    int upper;
    int unit;
    int cargo_unit;
    register int slot;
    int n;

    if (y < 0x132)
        upper = 1;
    else
        upper = 0;
    if (upper == 0) {
        n = x - 0x104;
        n = minimax(n, 0, 0x83);
        n = n + 0x10;
        slot = n / 0x22;
    } else {
        n = x - 0xf5;
        n = minimax(n, 0, 0x20e);
        slot = (n + 1) / 5;
        slot += 4;
    }
    slot = min_int_1010_e5d0(slot, g_carrier_count - 1);
    if (w->panel == 6 && g_selected_carrier != slot) {
        g_panel_mode = 2;
        g_selected_carrier = g_carrier_highlight = slot;
        colony_change_mode();
        goto out;
    } else if (w->panel == 6) {
        goto out;
    }
    if (w->panel == 7) {
        unit = nth_cargo_vessel(g_selected_carrier);
        cargo_unit = nth_cargo_vessel(slot);
        ColonyWin_AttemptShipToShip(w, unit, cargo_unit, g_drag_slot, shift_key());
        goto out;
    }
    if (g_colony_clicked != 0)
        g_dock_prev_carrier = g_selected_carrier;
    if (g_colony_drop_active != 0) {
        if (g_pedia_mode == 0) {
            if (g_selected_carrier == g_dock_prev_carrier && g_selected_carrier == slot) {
                colony_click_ship();
            } else {
                g_panel_mode = 2;
                g_selected_carrier = g_carrier_highlight = slot;
                colony_change_mode();
            }
        } else if (g_carrier_count != 0) {
            slot = nth_cargo_vessel(g_selected_carrier);
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, g_units[slot].type, &(DialogWin &)*w);
            reset_colony();
        }
    }
out:
    ;
}
