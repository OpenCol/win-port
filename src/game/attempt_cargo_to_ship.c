/* attempt_cargo_to_ship: candidate for 1010:d7fa (794 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 794, not the census 785. The trailing 9 bytes are the dead second epilogue.
 *
 * Loads goods from the current colony's warehouse onto a unit, reporting through the static colony
 * window at SEG20:0x25be -- the colony-to-unit counterpart of `unit_transfer_cargo`. It
 * refuses with a message when the colony has none of the goods (messages 6 and 7), when DGROUP:0xb2
 * is set and DGROUP:0xc14 is not (message 5, "!"), or when the unit has no room for them
 * (message 4, "!"). With `enabled` clear it stops silently and answers 0 without loading. Otherwise
 * the amount is at most 100, at most what the colony holds and at most the room the unit has, and
 * with `ask` set the player confirms it through "HOWMUCH1" against the limit at SEG31:0x2d4. The
 * load is reported as "<amount> <goods> ... <unit type>", and the colony window is refreshed when
 * DGROUP:0xb2 is set.
 *
 * The answer is 1 when nothing was loaded because of a refusal or a cancelled prompt, and 0 when
 * goods moved or `enabled` was clear. The value `cargo_to_ship` returns is stored and
 * never read.
 *
 * RENAMED 2026-09-26 from colony_load_unit: the Mac build's attempt_cargo_to_ship(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (callee AddNumberToMessage; callee AddToMessage; callee BackspaceMessage; callee CancelMessage; callee colony_activate_message), the runner-up gets 8, and the arity agrees (4). The runner-up is colony_unload_unit (8), which takes three arguments to this one's four and is attempt_cargo_to_colony(short, short, short) in the same batch.
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "records.h"

typedef struct { char pad[1]; } ColonyWin;

extern int cargo_holds_free(int u, int goods, int *room);
extern void popup_say(int slot, int word);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int cargo_to_ship(int u, int goods, int max);
#include "classes.h"
extern void ColChild_CancelMessage(ColChild *child, int a);

extern void ColChild_AddToMessage(ColChild *child, int word);
extern void ColChild_BackspaceMessage(ColChild *w);
extern void ColChild_AddStringToMessage(ColChild *message, char *s);

/* same object: reached by the 4-byte `push cs; call ` */

extern void set_text_arg_i_1010_ebb0(int slot, int v);

extern void colony_change_mode(void);

int attempt_cargo_to_ship(register int u, register int goods, int enabled, int ask)
{
    int r;
    int got;
    int n;
    int requested;

    r = 1;
    if (g_cur_colony_rec->stock[goods] < 1) {
        ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
        colony_message(CMSG_NOT_ENOUGH);
        ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
        colony_message(CMSG_TO_FILL_CARGO_HOLD);
        colony_activate_message(3, 0x78L);
        goto out;
    }
    if (g_colony_window_open != 0 && g_carrier_count == 0) {
        ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
        colony_message(CMSG_NOTHING_TO_TRANSPORT);
        ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
        ColChild_BackspaceMessage((ColChild *)&g_colony_window);
        ColChild_AddStringToMessage((ColChild *)&g_colony_window, "!");
        colony_activate_message(3, 0x78L);
        goto out;
    }
    if (enabled == 0) {
        r = 0;
        goto out;
    }
    if (cargo_holds_free(u, goods, &n) == 0) {
        ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
        colony_message(CMSG_NO_ROOM_FOR);
        ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
        ColChild_BackspaceMessage((ColChild *)&g_colony_window);
        ColChild_AddStringToMessage((ColChild *)&g_colony_window, "!");
        colony_activate_message(3, 0x78L);
        goto out;
    }
    n = min_int_1010_e5d0(n, 100);
    n = min_int_1010_e5d0(n, g_cur_colony_rec->stock[goods]);
    if (ask != 0) {
        popup_say(0, g_goods_name_word[goods]);
        popup_say(1, g_unit_types[g_units[u].type].name);
        set_text_arg_i_1010_ebb0(0, n);
        if (popup_ask_number(g_game_txt, "HOWMUCH1", n) != 0)
            goto out;
        requested = minimax((int)g_text_args[0], 0, n);
        n = min_int_1010_e5d0(requested, n);
        if (n <= 0)
            goto out;
    }
    got = cargo_to_ship(u, goods, n);
    ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
    ColChild_AddNumberToMessage((ColChild *)&g_colony_window, g_cargo_amount);
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_goods_name_word[goods]);
    colony_message(CMSG_MOVED_TO);
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_unit_types[g_units[u].type].name);
    colony_activate_message(1, 0x78L);
    if (g_colony_window_open != 0)
        colony_change_mode();
    r = 0;
out:
    return r;
}
