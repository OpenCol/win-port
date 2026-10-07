/* stack_units: candidate for 1038:35fc (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: stack_units
 *
 * A ship (type 13..18) goes to stack_ship; anything else is moved with
 * relocate_unit(u, -2, -2).  Nothing for a negative u.  Both helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_settle_or_move: the Mac build's stack_units(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller combat_fight; caller move_is_legal; caller move_unit; callee relocate_unit; callee stack_ship), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int g_stub_a;

extern void stack_ship(int u);
extern void relocate_unit(int u, int x, int y);

void stack_units(int u)
{
    if (u < 0)
        goto done;
    if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
        stack_ship(u);
    else
        relocate_unit(u, -2, -2);
done:
    ;
}
