/* compute_colony_dynamic: candidate for 1018:8c66 (22 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: compute_colony_dynamic
 *
 * Two calls and nothing else: recount the units on the colony's tile (reset_colony_units) and
 * recompute the colony's production for the turn (compute_colony_yield).  Both are
 * 4-byte `push cs / call ` sites, so both are defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_refresh_all: the Mac build's compute_colony_dynamic(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_change_mode; caller colony_change_selected; caller compute_colony_parameters; callee compute_colony_yield), the runner-up gets 1, and the arity agrees (0).
 */

#include "colonization.h"

extern int g_stub_a, g_stub_b;

extern void reset_colony_units(void);
extern void compute_colony_yield(void);

void compute_colony_dynamic(void)
{
    reset_colony_units();
    compute_colony_yield();
}
