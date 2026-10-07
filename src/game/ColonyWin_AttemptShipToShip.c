/* ColonyWin_AttemptShipToShip: candidate for 1010:b882 (657 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ColonyWin_AttemptShipToShip
 *
 * Moves one cargo slot from one unit to another on the colony dock, and reports it.
 * The single caller (1010:6c4c, the colony drag handler for panel kind 7) picks both
 * units with nth_cargo_vessel, passes the dragged slot from the byte at
 * DGROUP:0x4bb1 and takes `ask` from shift_key.
 *
 * The amount is clamped three times: to the room the receiver has for that goods
 * (cargo_holds_free, which also refuses the move outright -> message 4), to
 * 100, and to what the source slot actually holds.  With `ask` set the player is
 * offered "HOWMUCH3" with the three name slots filled in -- goods, from-unit type,
 * to-unit type -- and a cancel there abandons the move; the answer is then clamped
 * again through minimax against `g_text_args[0]`.
 *
 * unload_cargo empties the slot and leaves the amount it removed in
 * g_cargo_amount; anything over the agreed n is handed straight back to the source
 * unit.  A receiver that is not on a trade route (orders 2) has its orders cleared to 0.
 *
 * The census size is 648: nine bytes short of the dead epilogue, as usual (rule 44).
 *
 * RENAMED 2026-09-27 from unit_transfer_cargo_1010_b882: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 3. HOWMUCH3 is not in Mac LoadColony. Mac callgraph's LoadColony span 0x4ca0..0x50ee (1102 bytes) is two functions. CODE 30 has LINK 0x4ca0 .. RTS 0x4f04, followed by the long-form trailer 0x80 0x22 'AttemptShipToShip__9ColonyWinFssss'. Then LINK 0x4f2c .. RTS 0x50d0 is followed by 'LoadColony__9ColonyWinFv'. tools/mac_symbols.py reads only the short 0x81-0x9f form, so it missed the first name and the span took LoadColony's. The callgraph seq for 'LoadColony' is visibly two functions concatenated: first the transfer calls (get_cargo, cargo_holds_free, colony_error, popup_say x3, popup_ask_number, minimax, unload_cargo, stow_cargo x2, ColonyMessage, ColonyActivateMessage, colony_change_mode), then an EH frame with GRPort ctor/Init, Sprite::Draw x2, TextColor, DrawRect x4. The first half is b882's callee set: get_cargo x2, cargo_holds_free, colony_error x2, popup_say x3, popup_ask_number, minimax, unload_cargo, stow_cargo x2, ColonyWin_ColonyActivateMessage, ColonyWin_ColonyMessage, colony_change_mode. Callers: the Mac range start is called only by anon_13_3806 (minimax, nth_cargo_vessel x2, shift_key, then the call, then colony_click_ship). b882's only caller is colony_dock_click (1010:6b24), which the header says picks both units with nth_cargo_vessel and takes 'ask' from shift_key. Arity: Mac this+(short,short,short,short) against (void *w, from, to, slot, ask). Size: 612 Mac against 657. Layout: Mac CODE 30 runs ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony. Windows 1010 runs ..., ColonyWin_AttemptCargoToShip b19d, ColonyWin_AttemptCargoToColony b508, b882, bb30. Strings: HOWMUCH3 is on both sides. Naming follows the Class_Method convention (ColonyWin_colony_hammer_buy, EuropeWin_EuropeMessage). No collision.
 */

#include "colonization.h"

#include "units.h"
#include "w16types.h"
typedef struct { char pad[1]; } ColonyWin;
#include "records.h"

extern int get_cargo(int u, int slot);
extern int cargo_holds_free(int u, int goods, int *room);
extern void colony_error(int idx, long ms);
extern int get_cargo_amount(int u, int slot);
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int unload_cargo(int u, int slot);
extern int stow_cargo(int unit, int goods, int amount);
#include "classes.h"
extern void ColChild_CancelMessage(ColChild *child, int a);

extern void ColChild_AddToMessage(ColChild *child, int word);

/* same object: reached by the 4-byte `push cs; call ` */

extern void set_text_arg_i_1010_ebb0(int slot, int v);
extern void ColonyWin_ColonyMessage(void *w, int n);
extern void ColonyWin_ColonyActivateMessage(void *w, int kind, long ms);
extern void colony_change_mode(void);

int ColonyWin_AttemptShipToShip(ColonyWin *w, register int from, int to, int slot, int ask)
{
    int r;
    int cargo_amount;
    int cap;
    register int goods;

    r = 1;
    goods = get_cargo(from, slot);
    if (cargo_holds_free(to, goods, &cargo_amount) == 0) {
        colony_error(4, 0x78L);
        goto out;
    }
    cargo_amount = min_int_1010_e5d0(cargo_amount, 0x64);
    cargo_amount = min_int_1010_e5d0(cargo_amount, get_cargo_amount(from, slot));
    if (ask != 0) {
        goods = get_cargo(from, slot);
        popup_say(0, g_goods_name_word[goods]);
        popup_say(1, g_unit_types[g_units[from].type].name);
        popup_say(2, g_unit_types[g_units[to].type].name);
        set_text_arg_i_1010_ebb0(0, cargo_amount);
        if (popup_ask_number(g_game_txt, "HOWMUCH3", cargo_amount) != 0)
            goto out;
        cap = minimax((int)g_text_args[0], 0, cargo_amount);
        cargo_amount = min_int_1010_e5d0(cap, cargo_amount);
        if (cargo_amount <= 0)
            goto out;
    }
    goods = unload_cargo(from, slot);
    if (goods < 0) {
        colony_error(0xb, 0x78L);
        goto out;
    }
    if (g_cargo_amount > cargo_amount) {
        stow_cargo(from, goods, g_cargo_amount - cargo_amount);
        g_cargo_amount = cargo_amount;
    }
    stow_cargo(to, goods, g_cargo_amount);
    if (g_units[to].orders != ORDERS_TRADE_ROUTE)
        g_units[to].orders = ORDERS_NONE;
    ColChild_CancelMessage((ColChild *)w, 1);
    ColChild_AddNumberToMessage((ColChild *)w, g_cargo_amount);
    ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
    ColonyWin_ColonyMessage(w, 2);
    ColChild_AddToMessage((ColChild *)w, g_unit_types[g_units[to].type].name);
    ColonyWin_ColonyActivateMessage(w, 1, 0x78L);
    colony_change_mode();
    r = 0;
out:
    return r;
}
