/* stow_cargo: candidate for 1018:7d1c (252 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: stow_cargo
 *
 * Loads `amount` of `goods` onto unit `u`: first tops up every slot already
 * holding that goods (to 100 each), then, if something is left and the unit
 * type's hold (UnitType+7) has room, opens a new slot for the rest.
 * Returns the slot count before the new slot (uninitialised when nothing was
 * left over -- the shipped code returns whatever is in that frame slot).
 *
 * RENAMED 2026-09-26 from unit_add_cargo: the Mac build's stow_cargo(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller attempt_cargo_to_colony; caller perform_orders_load; caller plan_move; caller special_kill; caller tribe_trade), the runner-up gets 3, and the arity agrees (3).
 */

#include "colonization.h"

#include "records.h"

extern int get_cargo(int u, int slot);

extern int get_cargo_amount(int u, int slot);

extern void set_cargo_amount(int u, int slot, int amt);

extern int set_cargo(int u, int slot, int type);

int stow_cargo(int u, int goods, int amount)
{
    register int cargo_slot;
    int count;
    int have;
    int room;
    int take;

    for (cargo_slot = 0; cargo_slot < g_units[u].cargo_count; cargo_slot++) {
        if (get_cargo(u, cargo_slot) == goods) {
            have = get_cargo_amount(u, cargo_slot);
            room = 100 - have;
            if (room != 0) {
                take = min_int_1018_b329(room, amount);
                set_cargo_amount(u, cargo_slot, have + take);
                amount -= take;
            }
        }
    }
    if (amount != 0) {
        count = g_units[u].cargo_count;
        if (g_unit_types[g_units[u].type].holds > count) {
            set_cargo(u, count, goods);
            set_cargo_amount(u, count, amount);
            g_units[u].cargo_count++;
        }
    }
    return count;
}
