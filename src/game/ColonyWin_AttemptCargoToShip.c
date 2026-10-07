/* ColonyWin_AttemptCargoToShip: candidate for 1010:b19d (875 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_AttemptCargoToShip
 * name-source: sibling-build
 *
 * Loading goods from a colony onto a unit, with all four ways it can refuse. The colony has
 * none of that good, the game is in the state DGROUP:0xb2 marks with no carrier at 0xc14, the
 * unit has no room -- each gets its own message and returns 1 -- and a `go` of zero returns 0
 * without saying anything.
 *
 * Otherwise the amount is clamped twice, to 100 and to what the colony actually holds, and
 * when `ask` is set the player is asked for a number: "HOWMUCH1" at DGROUP:0xa99, with the
 * goods name in slot 0 and the unit's own name in slot 1. The answer is clamped again through
 * `minimax` against SEG31:0x2d4, and zero or less abandons the load.
 *
 * THE RETURN VALUE IS NOT SUCCESS. It is 1 on every refusal AND on the cancelled prompt, and 0
 * both when `go` was zero and after a load that WORKED. So the caller is being told "I said
 * something, redraw" rather than "it happened".
 *
 * `g_cargo_amount` at DGROUP:0x4bd2 is what the loader leaves behind, and it -- not the
 * requested amount -- is what the success message prints.
 *
 * The unit-type record is at SEG20:0x7ac8 with a WORD at +0: its text id. `1028:3296` and
 * `1040:1de3` read the same array two bytes further on, so their `0x7aca` is this record's +2.
 *
 * RENAMED 2026-09-27 from colony_load_prompt: Mac ColonyWin::AttemptCargoToShip(short, short, short, short), 794 bytes, against 875. Caller: ColonyMouseMarket on both. The 14 Mac callees are the Windows callee set, the ColChild message methods, b048, b08e, cargo_holds_free, cargo_to_ship, colony_change_mode, minimax, popup_ask_number, popup_say and set_colony, with min_int/set_text_arg as the Windows inline helpers. Arity: this+4 = (w, u, goods, go, ask). Layout: the Mac CODE 30 run Close, Done, DrawButtons, ColonyActivateMessage, ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony, UpdateColony is the Windows 1010 run ColonyWin_Close, Done, DrawButtons, b048, b08e, [colony_goods_message, Windows-only], b11a, b19d, b508, ColonyWin_AttemptShipToShip, ColonyWin_LoadColony, bd3f.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void set_colony(int colony);
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int idx);
extern void ColChild_BackspaceMessage(ColChild *w);

extern void ColChild_AddStringToMessage(ColChild *message, char *s);
extern int cargo_holds_free(int u, int g, int *room);
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int cargo_to_ship(int u, int g, int max);

/* same object: 4-byte `push cs; call ` */
extern "C" void ColonyWin_ColonyMessage(ColonyWin *w, int n);
extern "C" void ColonyWin_ColonyActivateMessage(ColonyWin *w, int kind, long ms);

extern "C" void set_text_arg_i_1010_ebb0(int slot, int v);
extern "C" void colony_change_mode(void);

int ColonyWin_AttemptCargoToShip(ColonyWin *w, register int u, register int goods,
                                     int go, int ask)
{
    int r;
    int amount;
    int cap;

    r = 1;
    set_colony(w->colony);
    if (g_colonies[w->colony].stock[goods] < 1) {
        ColChild_CancelMessage((ColChild *)w, 1);
        ColonyWin_ColonyMessage(w, 6);
        ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
        ColonyWin_ColonyMessage(w, 7);
        ColonyWin_ColonyActivateMessage(w, 3, 0x78L);
        goto out;
    }
    if (g_colony_window_open != 0 && g_carrier_count == 0) {
        ColChild_CancelMessage((ColChild *)w, 1);
        ColonyWin_ColonyMessage(w, 5);
        ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage((ColChild *)w);
        ColChild_AddStringToMessage((ColChild *)w, "!");
        ColonyWin_ColonyActivateMessage(w, 3, 0x78L);
        goto out;
    }
    if (go == 0) {
        r = 0;
        goto out;
    }
    if (cargo_holds_free(u, goods, &amount) == 0) {
        ColChild_CancelMessage((ColChild *)w, 1);
        ColonyWin_ColonyMessage(w, 4);
        ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage((ColChild *)w);
        ColChild_AddStringToMessage((ColChild *)w, "!");
        ColonyWin_ColonyActivateMessage(w, 3, 0x78L);
        goto out;
    }
    amount = min_int_1010_e5d0(amount, 0x64);
    amount = min_int_1010_e5d0(amount, g_colonies[w->colony].stock[goods]);
    if (ask != 0) {
        popup_say(0, g_goods_name_word[goods]);
        popup_say(1, g_unit_types[g_units[u].type].name);
        set_text_arg_i_1010_ebb0(0, amount);
        if (popup_ask_number(g_game_txt, "HOWMUCH1", amount) != 0)
            goto out;
        cap = minimax((int)g_text_args[0], 0, amount);
        amount = min_int_1010_e5d0(cap, amount);
        if (amount <= 0)
            goto out;
    }
    cargo_to_ship(u, goods, amount);
    ColChild_CancelMessage((ColChild *)w, 1);
    ColChild_AddNumberToMessage((ColChild *)w, g_cargo_amount);
    ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
    ColonyWin_ColonyMessage(w, 2);
    ColChild_AddToMessage((ColChild *)w, g_unit_types[g_units[u].type].name);
    ColonyWin_ColonyActivateMessage(w, 1, 0x78L);
    if (g_colony_window_open != 0)
        colony_change_mode();
    r = 0;
out:
    return r;
}
