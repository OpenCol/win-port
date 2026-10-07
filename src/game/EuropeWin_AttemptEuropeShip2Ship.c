/* EuropeWin_AttemptEuropeShip2Ship: candidate for 1048:2979 (752 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_AttemptEuropeShip2Ship
 * name-source: sibling-build
 * size-source: 752, not the census 743. The trailing 9 bytes are the dead second epilogue.
 *
 * Moves one cargo slot from one unit to another in port in Europe, and reports it on the Europe
 * window -- the Europe screen's copy of `ColonyWin_AttemptShipToShip`, with the window's own
 * nation made current first and the message helpers of segment 1048. The amount is clamped to the
 * receiver's room (refused outright with message 4 and "!"), to 100 and to what the slot holds;
 * with `ask` set the player confirms it through "HOWMUCH3" against SEG31:0x2d4. What is not taken
 * goes back to the source unit.
 *
 * Unlike the colony copy it does not wake the receiver, and it calls 1048:4e81 once the move is
 * reported. The refusal names the goods from the RECEIVER's slot, not the source's.
 *
 * The source unit is copied into a register local, so it sits in SI with the goods in DI
 * (rule 67).
 *
 * RENAMED 2026-09-27 from europe_transfer_cargo: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 3. It is also one of the three HOWMUCH3 holders. Mac CODE 33 LINK 0xe84 .. RTS 0x111c is closed by the long-form name 'AttemptEuropeShipToShip__9EuropeWinFssss', which the extractor missed. The callgraph calls it anon_33_e84. Its seq is set_europe, get_cargo, cargo_holds_free, get_cargo, CancelMessage, EuropeMessage, AddToMessage, BackspaceMessage, .., EuropeActivateMessage, get_cargo_amount x2, get_cargo, popup_say x3, popup_ask_number, minimax, unload_cargo, EuropeError, stow_cargo x2, CancelMessage, AddNumberToMessage, AddToMessage, EuropeMessage, .., europe_change_mode. 2979's callees are the same set: set_europe, get_cargo x3, cargo_holds_free, get_cargo_amount, popup_say x3, popup_ask_number, minimax, unload_cargo, EuropeWin_EuropeError (its only caller!), stow_cargo x2, msg_activate_redraw, europe_change_mode. Arity: this+(short x4) against (EuropeWin *w, src, to, slot, ask). Size: 664 Mac against 752. Layout: Mac CODE 33 runs EuropeActivateMessage(0x36c), AttemptCargoFromEurope(0x3d0), AttemptCargoToEurope(0x8d0), AttemptEuropeShipToShip(0xe84), EuropeMouseMarket(0x114a). Windows 1048 runs msg_activate_redraw 1c79, europe_buy_cargo 1cbd, europe_sell_cargo 2251, 2979, europe_market_click 2c69. It is third of three on both sides. Callers: Mac EuropeMouseUnits against europe_ships_click. That is the one soft point, because the Windows click-handler names in 1048:2c69..319d are not Mac-bound; it does not affect this binding. No collision. ADDED BY THE CONST BATCH: the one soft point (callers) is now closed. With BSR.L decoded, Mac AttemptEuropeShipToShip is called from EuropeMouseShipsProper, which is 1048:319d europe_ships_click in this batch, and that is 2979's Windows caller.
 *
 * RENAMED 2026-09-27 from EuropeWin_AttemptEuropeShipToShip: Borland C++ keeps 32 significant characters of an identifier, and EuropeWin_AttemptEuropeShipToShip is 33: the compiled PUBDEF came out truncated and no longer matched its own name. The Mac name is otherwise kept.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int get_cargo(int u, int slot);
extern int cargo_holds_free(int u, int goods, int *room);
extern int get_cargo_amount(int u, int slot);
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int unload_cargo(int u, int slot);
extern int stow_cargo(int unit, int goods, int amount);

/* 5-byte NOP form, so extern (rule 47) */
extern void set_europe(int n);
extern void europe_change_mode(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_CancelMessage(EuropeWin *child, int a);
extern "C" void EuropeWin_EuropeMessage(EuropeWin *win, int n);
extern "C" void ColChild_AddToMessage(EuropeWin *child, int word);
extern "C" void ColChild_BackspaceMessage(EuropeWin *w);
extern "C" void ColChild_AddStringToMessage(EuropeWin *message, char *s);
extern "C" void EuropeWin_EuropeActivateMessage(EuropeWin *win, int kind, long ms);

extern "C" void set_text_arg_i_1048_db6a(int i, int v);
extern "C" void EuropeWin_EuropeError(EuropeWin *win, int word, long ms);

int EuropeWin_AttemptEuropeShip2Ship(EuropeWin *w, int src, int to, int slot, int ask)
{
    register int from = src;
    register int cargo;
    int r;
    int goods;
    int cargo_amount;
    int chosen;

    r = 1;
    set_europe(w->nation);
    goods = get_cargo(from, slot);
    if (cargo_holds_free(to, goods, &cargo_amount) == 0) {
        goods = get_cargo(to, slot);
        ColChild_CancelMessage(w, 1);
        EuropeWin_EuropeMessage(w, 4);
        ColChild_AddToMessage(w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage(w);
        ColChild_AddStringToMessage(w, "!");
        EuropeWin_EuropeActivateMessage(w, 3, 0x78L);
        goto out;
    }
    cargo_amount = min_int_1048_db44(cargo_amount, 0x64);
    cargo_amount = min_int_1048_db44(cargo_amount, get_cargo_amount(from, slot));
    if (ask != 0) {
        cargo = get_cargo(from, slot);
        popup_say(0, g_goods_name_word[cargo]);
        popup_say(1, g_unit_types[g_units[from].type].name);
        popup_say(2, g_unit_types[g_units[to].type].name);
        set_text_arg_i_1048_db6a(0, cargo_amount);
        if (popup_ask_number(g_game_txt, "HOWMUCH3", cargo_amount) != 0)
            goto out;
        chosen = minimax((int)g_text_args[0], 0, cargo_amount);
        cargo_amount = min_int_1048_db44(chosen, cargo_amount);
        if (cargo_amount <= 0)
            goto out;
    }
    cargo = unload_cargo(from, slot);
    if (cargo < 0) {
        EuropeWin_EuropeError(w, 0xb, 0x78L);
        goto out;
    }
    if (g_cargo_amount > cargo_amount) {
        stow_cargo(from, cargo, g_cargo_amount - cargo_amount);
        g_cargo_amount = cargo_amount;
    }
    stow_cargo(to, cargo, g_cargo_amount);
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, g_cargo_amount);
    ColChild_AddToMessage(w, g_goods_name_word[cargo]);
    EuropeWin_EuropeMessage(w, 2);
    ColChild_AddToMessage(w, g_unit_types[g_units[to].type].name);
    EuropeWin_EuropeActivateMessage(w, 1, 0x78L);
    europe_change_mode();
    r = 0;
out:
    return r;
}
