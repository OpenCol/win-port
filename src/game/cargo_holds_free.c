/* cargo_holds_free: candidate for 1018:7eba (190 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: cargo_holds_free
 *
 * How much of `goods` unit `u` can still take.  Free slots (the type's hold,
 * UnitType+7, minus the slots in use) give 100 each; only when there is
 * no free slot are the part-filled slots of that goods counted, each adding
 * its remaining space to *room.  Returns the number of usable slots.
 *
 * RENAMED 2026-09-26 from unit_cargo_room: the Mac build's cargo_holds_free(short, short, short*), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller attempt_cargo_to_ship; callee get_cargo; callee get_cargo_amount), the runner-up gets 2, and the arity agrees (3). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

extern int get_cargo(int u, int slot);

extern int get_cargo_amount(int u, int slot);

int cargo_holds_free(int u, int goods, int *room)
{
    register int cargo_slot;
    int free;

    free = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
    *room = free * 100;
    if (free == 0) {
        for (cargo_slot = 0; cargo_slot < g_units[u].cargo_count; cargo_slot++) {
            if (get_cargo(u, cargo_slot) == goods) {
                if (get_cargo_amount(u, cargo_slot) < 100) {
                    free++;
                    *room += 100 - get_cargo_amount(u, cargo_slot);
                }
            }
        }
    }
    return free;
}
