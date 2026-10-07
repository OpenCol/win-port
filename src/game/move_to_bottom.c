/* move_to_bottom: candidate for 1038:1dfa (217 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: move_to_bottom
 *
 * Takes a carried unit (link2 >= 0) off its carrier and appends it to the
 * tile chain: the chain tail (or the next unit when the tail is itself),
 * the detach (pick_up_unit), the 26-chain tail's link2 pointed at
 * it, its link set, link2 cleared and position restored.  The four
 * helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_unload_carrier: the Mac build's move_to_bottom(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller cargo_check; caller combat_fight; caller europe_new_turn; callee next_unit; callee top_unit), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int top_unit(int u);
extern int next_unit(int u);
extern void pick_up_unit(int u);
extern int bottom_unit(int u);

void move_to_bottom(int u)
{
    int p;
    int x;
    int y;
    int bottom;

    if (g_units[u].link2 >= 0) {
        x = g_units[u].x;
        y = g_units[u].y;
        p = top_unit(u);
        if (p == u)
            p = next_unit(u);
        pick_up_unit(u);
        bottom = bottom_unit(p);
        g_units[bottom].link2 = u;
        g_units[u].link = bottom;
        g_units[u].link2 = -1;
        g_units[u].x = x;
        g_units[u].y = y;
    }
}
