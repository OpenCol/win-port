/* subtract_gold: candidate for 1018:40c5 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: subtract_gold
 *
 * Takes gold away from a nation by adding the negated amount, so the same clamp applies. The adder is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from nation_take_gold: the Mac build's subtract_gold(short, long), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller buy_this; caller cargo_from_europe; caller orders_clear; caller orders_road; caller set_field), the runner-up gets 3, the arity agrees (2), and the Mac body is 1.39x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

extern long g_stub_l;

extern void add_gold(int n, long amount);

void subtract_gold(int nation, long amount)
{
    add_gold(nation, 0L - amount);
}
