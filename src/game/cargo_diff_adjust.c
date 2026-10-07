/* cargo_diff_adjust: candidate for 1048:4f66 (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A percentage of the caller's value scaled by the current nation's rate: the
 * byte at SEG20:0x77a8 less two for a human-controlled European nation, -2 otherwise,
 * times sixteen, times the argument, over a hundred.  Both arms load AX and
 * the register variable is written once after the join, so the choice is a
 * conditional expression rather than an if.
 *
 * RENAMED 2026-09-26 from scaled_rate: the Mac build's cargo_diff_adjust(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in traffic_from_europe, traffic_to_europe -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (2.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int cargo_diff_adjust(int amount)
{
    register int r;

    r = (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) ? g_game.difficulty - 2 : -2;
    r = r << 4;
    r = r * amount / 100;
    return r;
}
