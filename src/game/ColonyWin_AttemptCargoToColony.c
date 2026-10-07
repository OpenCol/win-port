/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_AttemptCargoToColony: candidate for 1010:b508 (890 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_AttemptCargoToColony
 * name-source: sibling-build
 *
 * Unloads one kind of cargo from a unit into the colony's warehouse.  With
 * `ask` set it first fills the HOWMUCH2 dialog (goods name, unit name, colony
 * name, amount) and clamps the answer; if the warehouse would overflow it
 * puts up WAREHOUSEFULL and goes on only if the answer is 2.  Returns 0 when
 * cargo moved, 1 otherwise.  The six -called helpers are 4-byte
 * `push cs / call ` sites, so the file defines them before the target
 * (sliced out by `symbol:`).
 *
 * RENAMED 2026-09-26 from warehouse_capacity_check: The body unloads one kind of cargo from a unit into the colony's warehouse. It finds the cargo slot (and says so if there is none), optionally asks HOWMUCH2 for the amount, then calls unload_cargo, adds the amount to colony stock[goods], re-stows any remainder and shows the 'N goods unloaded in colony' message. It returns 0 when the goods moved. ColonyWin_ColonyMouseShipsCargo calls it for a drop on the warehouse panel. The WAREHOUSEFULL capacity test is just one gate along the way, so the old name picks out a side check instead of the job. The new name pairs it with its mirror, ColonyWin_AttemptCargoToShip (HOWMUCH1).
 *
 * RENAMED 2026-09-27 from colony_unload_prompt: Mac ColonyWin::AttemptCargoToColony(short, short, short), 788 bytes, against 890. Caller: ColonyMouseShipsCargo on both. Callees: find_cargo, get_warehouse, popm, stow_cargo, unload_cargo, minimax, popup_ask_number, popup_say, colony_change_mode, b048/b08e and the ColChild message methods, the same set on both. String: WAREHOUSEFULL. Arity: this+3 = (w, unit, goods, ask). Layout: the Mac CODE 30 run Close, Done, DrawButtons, ColonyActivateMessage, ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony, UpdateColony is the Windows 1010 run ColonyWin_Close, Done, DrawButtons, b048, b08e, [colony_goods_message, Windows-only], b11a, b19d, b508, ColonyWin_AttemptShipToShip, ColonyWin_LoadColony, bd3f.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void set_colony(int colony);
extern int get_warehouse(void);
extern int find_cargo(int unit, int goods);
extern int unload_cargo(int unit, int slot);
extern int stow_cargo(int unit, int goods, int amount);
extern "C" void ColChild_CancelMessage(ColChild *child, int a);   /* PORT: C linkage, as defined (W2) */
extern "C" void ColChild_AddToMessage(ColChild *child, int idx);   /* PORT: C linkage, as defined (W2) */

extern "C" void ColChild_AddStringToMessage(ColChild *message, char *s);   /* PORT: C linkage, as defined (W2) */
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern "C" int popm(char *msg, int n);   /* PORT: C linkage, as defined (W2) */

extern "C" void ColonyWin_ColonyMessage(ColonyWin *w, int n);

extern "C" void ColonyWin_ColonyActivateMessage(ColonyWin *w, int kind, long ms);

extern "C" void set_text_arg_i_1010_ebb0(int i, int v);

extern "C" void colony_change_mode(void);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
int ColonyWin_AttemptCargoToColony(ColonyWin *w, int unit, int goods, int ask)
{
    int result = 1;
    int slot;
    int cap;
    int requested;
    int amt;
    int rem = 0;

    set_colony(w->colony);
    slot = find_cargo(unit, goods);
    if (slot < 0) {
        if (g_colony_window_open != 0) {
            ColChild_CancelMessage((ColChild *)w, 1);
            ColonyWin_ColonyMessage(w, 9);
            ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
            ColonyWin_ColonyMessage(w, 10);
            ColonyWin_ColonyActivateMessage(w, 3, 120L);
        }
    } else {
        if (ask != 0) {
            popup_say(0, g_goods_name_word[goods]);
            popup_say(1, g_unit_types[g_units[unit].type].name);
            set_name_slot_1010_e658(2, g_colonies[w->colony].name);
            set_text_arg_i_1010_ebb0(0, g_cargo_amount);
            if (popup_ask_number(g_game_txt, "HOWMUCH2", g_cargo_amount) != 0)
                goto done;
            requested = minimax((int)g_text_args[0], 0, g_cargo_amount);
            rem = g_cargo_amount - requested;
            g_cargo_amount = min_int_1010_e5d0(requested, g_cargo_amount);
            if (requested <= 0)
                goto done;
        }
        cap = get_warehouse();
        if (g_colonies[w->colony].stock[goods] + g_cargo_amount > cap && goods != 0 && g_colony_window_open != 0) {
            set_name_slot_1010_e658(0, g_colonies[w->colony].name);
            popup_say(1, g_goods_name_word[goods]);
            set_text_arg_i_1010_ebb0(0, g_colonies[w->colony].stock[goods]);
            set_text_arg_i_1010_ebb0(1, cap);
            set_text_arg_i_1010_ebb0(2, g_cargo_amount);
            if (popm("WAREHOUSEFULL", 5) != 2)
                goto done;
        }
        amt = g_cargo_amount;
        unload_cargo(unit, slot);
        g_colonies[w->colony].stock[goods] += amt;
        stow_cargo(unit, goods, rem);
        g_cargo_amount = amt;
        ColChild_CancelMessage((ColChild *)w, 1);
        ColChild_AddNumberToMessage((ColChild *)w, g_cargo_amount);
        ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
        ColonyWin_ColonyMessage(w, 2);
        ColChild_AddStringToMessage((ColChild *)w, g_colonies[w->colony].name);
        ColonyWin_ColonyActivateMessage(w, 1, 120L);
        if (g_colony_window_open != 0)
            colony_change_mode();
        result = 0;
    }
done:
    return result;
}
