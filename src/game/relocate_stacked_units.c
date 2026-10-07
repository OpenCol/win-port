/* relocate_stacked_units: candidate for 1038:1ed3 (62 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: relocate_stacked_units
 *
 * Moves every unit of u's chain with relocate_unit(u, a, b), stepping
 * to the next before the move.  The three helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_chain_relink: the Mac build's relocate_stacked_units(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller attempt_to_set_sail; caller combat_fight; caller move_unit; caller negotiate_europe; caller plan_move), the runner-up gets 4, and the arity agrees (3).
 */

#include "colonization.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);
extern void relocate_unit(int u, int x, int y);

void relocate_stacked_units(int u, int x, int y)
{
    register int unit;
    register int next;

    unit = top_unit(u);
    while (unit >= 0) {
        next = next_unit(unit);
        relocate_unit(unit, x, y);
        unit = next;
    }
}
