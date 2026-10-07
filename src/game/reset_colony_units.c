/* reset_colony_units: candidate for 1018:462f (184 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: reset_colony_units
 *
 * Recounts the units on the current colony's tile into DGROUP:0x4bb2..0x4bba:
 * the colonists (capped at 50), every unit, the units without a hold, the
 * chain's first unit; the selection index at 0x4bba is reset when it runs
 * past the hold-less count.  The colonist test is -called (in-file).
 *
 * RENAMED 2026-09-26 from count_tile_units: the Mac build's reset_colony_units(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_plan; caller set_job; callee is_colonist; callee next_unit; callee top_unit_in_square), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern int is_colonist(int u);

void reset_colony_units(void)
{
    register int u;

    g_tile_colonists = 0;
    g_tile_units = 0;
    g_tile_noncarriers = 0;
    g_tile_first_unit = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
    for (u = g_tile_first_unit; u >= 0; u = next_unit(u)) {
        if (is_colonist(u) != 0)
            g_tile_colonists++;
        if (g_unit_types[g_units[u].type].holds <= 0)
            g_tile_noncarriers++;
        g_tile_units++;
    }
    g_tile_colonists = min_int_1018_b329(50, g_tile_colonists);
    if (g_tile_noncarriers != 0 && g_tile_sel >= g_tile_noncarriers)
        g_tile_sel = 0;
}
