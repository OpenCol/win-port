/* attempt_cargo_to_colony: candidate for 1010:db14 (820 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 820, not the census 811. The trailing 9 bytes are the dead second epilogue.
 *
 * Unloads one kind of goods from a unit into the current colony's warehouse, the counterpart of
 * `attempt_cargo_to_ship` and reported the same way through the colony window at
 * SEG20:0x25be. A unit not carrying the goods is refused -- with messages 9 and 10 only when
 * DGROUP:0xb2 is set. With `ask` set the player chooses the amount through "HOWMUCH2", capped by
 * SEG31:0x2d4, and whatever is not taken stays on the unit. Anything but goods type 0 that would
 * take the warehouse past `get_warehouse` asks "WAREHOUSEFULL" first (again only with
 * DGROUP:0xb2 set), and only answer 2 goes ahead. The load is then taken off the unit whole, added
 * to the colony's stock, the remainder put back on the unit, and "<amount> <goods> ... <colony>"
 * reported.
 *
 * The answer is 0 when goods moved and 1 otherwise.
 *
 * RENAMED 2026-09-26 from colony_unload_unit: the Mac build's attempt_cargo_to_colony(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (caller colony_click_ship; callee AddNumberToMessage; callee AddToMessage; callee CancelMessage; callee colony_activate_message), the runner-up gets 8, and the arity agrees (3).
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "records.h"

typedef struct { char pad[1]; } ColonyWin;

extern int find_cargo(int u, int goods);
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int get_warehouse(void);
extern int popm(char *msg, int n);
extern int unload_cargo(int u, int slot);
extern int stow_cargo(int unit, int goods, int amount);
#include "classes.h"
extern void ColChild_CancelMessage(ColChild *child, int a);

extern void ColChild_AddToMessage(ColChild *child, int word);
extern void ColChild_AddStringToMessage(ColChild *message, char *s);

/* same object: reached by the 4-byte `push cs; call ` */

extern void set_text_arg_i_1010_ebb0(int slot, int v);

extern void colony_change_mode(void);

int attempt_cargo_to_colony(register int u, register int goods, int ask)
{
    int r;
    int slot;
    int cap;
    int chosen;
    int amount;
    int rest;

    r = 1;
    rest = 0;
    slot = find_cargo(u, goods);
    if (slot < 0) {
        if (g_colony_window_open != 0) {
            ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
            colony_message(CMSG_NO);
            ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
            colony_message(CMSG_IN_CARGO_HOLD);
            colony_activate_message(3, 0x78L);
        }
        goto out;
    }
    if (ask != 0) {
        popup_say(0, g_goods_name_word[goods]);
        popup_say(1, g_unit_types[g_units[u].type].name);
        set_name_slot_1010_e658(2, g_cur_colony_rec->name);
        set_text_arg_i_1010_ebb0(0, g_cargo_amount);
        if (popup_ask_number(g_game_txt, "HOWMUCH2", g_cargo_amount) != 0)
            goto out;
        chosen = minimax((int)g_text_args[0], 0, g_cargo_amount);
        rest = g_cargo_amount - chosen;
        g_cargo_amount = min_int_1010_e5d0(chosen, g_cargo_amount);
        if (chosen <= 0)
            goto out;
    }
    cap = get_warehouse();
    if (g_cur_colony_rec->stock[goods] + g_cargo_amount > cap && goods != 0 && g_colony_window_open != 0) {
        set_name_slot_1010_e658(0, g_cur_colony_rec->name);
        popup_say(1, g_goods_name_word[goods]);
        set_text_arg_i_1010_ebb0(0, g_cur_colony_rec->stock[goods]);
        set_text_arg_i_1010_ebb0(1, cap);
        set_text_arg_i_1010_ebb0(2, g_cargo_amount);
        if (popm("WAREHOUSEFULL", 5) != 2)
            goto out;
    }
    amount = g_cargo_amount;
    unload_cargo(u, slot);
    g_cur_colony_rec->stock[goods] += amount;
    stow_cargo(u, goods, rest);
    g_cargo_amount = amount;
    ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
    ColChild_AddNumberToMessage((ColChild *)&g_colony_window, g_cargo_amount);
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
    colony_message(CMSG_MOVED_TO);
    ColChild_AddStringToMessage((ColChild *)&g_colony_window, g_cur_colony_rec->name);
    colony_activate_message(1, 0x78L);
    if (g_colony_window_open != 0)
        colony_change_mode();
    r = 0;
out:
    return r;
}
