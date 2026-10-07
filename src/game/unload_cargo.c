/* unload_cargo: candidate for 1018:7e18 (162 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: unload_cargo
 *
 * Removes cargo slot `slot` from unit `u`: its amount is left in DGROUP:0x4bd2,
 * the later slots (type nibble and amount) move down one, and the count drops.
 * Returns the removed slot's goods type, or -1 for an empty slot (nothing moves).
 *
 * RENAMED 2026-09-26 from unit_drop_cargo_slot: the Mac build's unload_cargo(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller foreign_country; caller orders_overboard; caller perform_orders_unload; caller plan_move; caller special_kill), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

extern int get_cargo(int u, int slot);

extern int get_cargo_amount(int u, int slot);

extern void set_cargo_amount(int u, int slot, int amt);

extern int set_cargo(int u, int slot, int type);

int unload_cargo(int u, int slot)
{
    register int index;
    int cargo;

    cargo = get_cargo(u, slot);
    if (cargo < 0)
        goto done;
    g_cargo_amount = get_cargo_amount(u, slot);
    for (index = slot; index < g_units[u].cargo_count - 1; index++) {
        set_cargo(u, index, get_cargo(u, index + 1));
        set_cargo_amount(u, index, get_cargo_amount(u, index + 1));
    }
    g_units[u].cargo_count--;
done:
    return cargo;
}
