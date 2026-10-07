/* reset_europe_units: candidate for 1048:4c27 (125 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Counts the NON-SHIP units standing on the current tile into DGROUP:0x4cec, then drops the
 * selection index at 0x4cee back to zero if it now points past the end. An empty tile leaves
 * the index alone.
 *
 * "Non-ship" is types outside 0xd..0x12, the same range `ask_port` uses to
 * decide whether a unit sails.
 *
 * The tile is `top_unit_in_square(g_cur_nation - 20, g_cur_nation - 20)` -- both coordinates from
 * the one word at DGROUP:0x4cc8, exactly as `num_cargo_vessels` reads it.
 *
 * `compute_europe_parameters`, 40 bytes below, calls this after recounting the flagged
 * chain, so the two counts are maintained together.
 *
 * The type test is one `||`: the first comparison jumps INTO the increment and the second
 * jumps past it, both direct, which is the short-circuit and not two `if`s.
 *
 * THAT WORD IS THE CURRENT NATION INDEX. `set_europe` writes DGROUP:0x4cc8 with
 * the nation and 0x4cc4 with `&g_nations[n]`, and `nation - 20` in both
 * coordinates is that nation's off-board Europe square -- the same pair `attempt_to_set_sail`
 * relinks a ship to and `EuropeWin_DrawUnits` walks -- a deliberate encoding, not a shipped bug. The name here follows what the word IS; what this function does with it
 * is the game's own affair.
 *
 * RENAMED 2026-09-26 from recount_land_units_on_tile: the Mac build's reset_europe_units(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller attempt_to_set_sail; caller europe_recruit; caller europe_recruit_any; callee next_unit; callee top_unit_in_square), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

void reset_europe_units(void)
{
    register int u;

    g_land_unit_count = 0;
    for (u = top_unit_in_square(g_cur_nation - 20, g_cur_nation - 20); u >= 0;
         u = next_unit(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            g_land_unit_count++;
    }
    if (g_land_unit_count != 0) {
        if (g_land_unit_index >= g_land_unit_count)
            g_land_unit_index = 0;
    }
}
