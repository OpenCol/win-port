/* relocate_unit: candidate for 1038:1d92 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: relocate_unit
 *
 * pick_up_unit on the unit, then put_down_unit with the two
 * arguments: the unit is taken off one chain and put on another.  Both are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_detach_attach: the Mac build's relocate_unit(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller negotiate_europe; caller relocate_stacked_units; caller sort_stack_by_class), the runner-up gets 1, and the arity agrees (3).
 */

#include "colonization.h"

extern int g_stub_a;

extern void pick_up_unit(int u);
extern void put_down_unit(int u, int x, int y);

void relocate_unit(int u, int x, int y)
{
    pick_up_unit(u);
    put_down_unit(u, x, y);
}
