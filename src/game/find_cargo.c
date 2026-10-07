/* find_cargo: candidate for 1018:7f78 (111 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: find_cargo
 *
 * Finds the cargo slot of unit `u` that holds `goods`, stopping at the first
 * hit; -1 when none.  On a hit the slot's amount is left in DGROUP:0x4bd2 for
 * the caller (ColonyWin_AttemptCargoToColony reads it).  The two helpers are
 * -called, so they are defined in-file.
 *
 * RENAMED 2026-09-26 from unit_find_cargo_slot: the Mac build's find_cargo(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller attempt_cargo_to_colony; caller perform_orders_route; callee get_cargo; callee get_cargo_amount), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

extern int get_cargo(int u, int slot);

extern int get_cargo_amount(int u, int slot);

int find_cargo(int u, int goods)
{
    register int found = -1;
    register int i;
    int cargo;

    for (i = 0; found < 0 && i < g_units[u].cargo_count; i++) {
        cargo = get_cargo(u, i);
        if (cargo == goods)
            found = i;
    }
    if (found >= 0)
        g_cargo_amount = get_cargo_amount(u, found);
    return found;
}
