/* orders_hold: candidate for 1008:006f (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * end_unit_turn on the unit at SEG20:0x7794, the one the map module
 * calls the arrived unit.
 *
 * RENAMED 2026-09-26 from refresh_arrived_unit_1008_006f: the Mac build's orders_hold(void), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in GameBrains_KeyboardEvent -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (0). The call-graph vote had already put it first (2 vs 1).
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern void end_unit_turn(int u);

void orders_hold(void)
{
    end_unit_turn(g_game.selected_unit);
}
