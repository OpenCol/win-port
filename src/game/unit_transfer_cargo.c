/* unit_transfer_cargo: candidate for 1010:de48 (639 bytes).  Verdict: RELOCATABLE.
 * symbol: unit_transfer_cargo
 *
 * The same cargo transfer as `ColonyWin_AttemptShipToShip`, written against the static
 * colony window at SEG20:0x25be instead of a ColChild passed in -- so it takes four
 * arguments rather than five, and reports through the three -called colony message
 * helpers (`colony_error`, `colony_message`, `colony_activate_message`)
 * rather than through a window pointer.
 *
 * Nothing in the image calls it. It reads like the original of the pair, left behind when
 * the colony window became a parameter; its "HOWMUCH3" is a second copy of the literal at
 * DGROUP:0xbe4, the parameterised twin's being at DGROUP:0xab9.
 *
 * Clamps, refusal message 4, the destructive drop and hand-back through g_cargo_amount, and
 * the waking of a receiver that is not on a trade route are all identical -- see the twin's header.
 *
 * The census size is 630: nine bytes short of the dead epilogue (rule 44).
 *
 * RENAMED 2026-09-27 from unit_transfer_cargo_1010_de48: its family's other copy took the Mac build's own name in this batch (1030:091b SetDifficulty_LeftMouseClick / 1010:b882 ColonyWin_AttemptShipToShip), so this is now the family's only member and drops its address suffix (check_naming: a singleton carries no suffix).
 */

#include "colonization.h"

#include "units.h"
#include "labels.h"
#include "w16types.h"
#include "records.h"

typedef struct { char pad[1]; } ColonyWin;

extern int get_cargo(int u, int slot);
extern int cargo_holds_free(int u, int goods, int *room);
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

extern void colony_error(int idx, long ms);

int unit_transfer_cargo(register int from, int to, int slot, int ask)
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
    ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
    ColChild_AddNumberToMessage((ColChild *)&g_colony_window, g_cargo_amount);
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
    colony_message(CMSG_MOVED_TO);
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_unit_types[g_units[to].type].name);
    colony_activate_message(1, 0x78L);
    r = 0;
out:
    return r;
}
