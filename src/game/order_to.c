/* order_to: candidate for 1028:323f (87 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Puts a unit on a mission: the byte at Unit+7 from the caller, orders (+8)
 * set to 11, and the two bytes at +9 and +10 from the caller.
 *
 * RENAMED 2026-09-26 from unit_set_mission: the Mac build's order_to(short, short, short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in plan_move -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (4).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

void order_to(int u, int mission, int dest_x, int dest_y)
{
    g_units[u].mission = mission;
    g_units[u].orders = ORDERS_MISSION;
    g_units[u].dest_x = dest_x;
    g_units[u].dest_y = dest_y;
}
