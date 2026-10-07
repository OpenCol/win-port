/* compute_colony_parameters: candidate for 1018:8c7c (22 bytes).  Verdict: RELOCATABLE.
 * symbol: compute_colony_parameters
 * name-source: sibling-build
 *
 * Two calls and nothing else, the same shape as `compute_colony_dynamic` 22 bytes
 * before it: rebuild the colony screen, then recompute the colony through that neighbour. Both
 * are 4-byte `push cs / call ` sites, so both are defined in-file as stubs (rule 47).
 *
 * WHAT THE CALLERS ALREADY CALLED IT. `colony_panels_reset` says "recounts the colony
 * and resets the nine panel states", and `colony_add_unit_2` says it "refreshes" the
 * colony -- which is the pair of calls below, read from the outside. Twenty-four call sites had
 * it as `compute_colony_parameters` because nothing had reconstructed the two lines it is.
 *
 * RENAMED 2026-09-26 from colony_refresh_screen: the Mac build's compute_colony_parameters(void), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawColony; caller colony_add_unit_2; caller colony_add_unit; caller colony_plan; caller create_colony), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

extern int g_stub_a, g_stub_b;

extern void compute_colony_base_parameters(void);
extern void compute_colony_dynamic(void);

void compute_colony_parameters(void)
{
    compute_colony_base_parameters();
    compute_colony_dynamic();
}
