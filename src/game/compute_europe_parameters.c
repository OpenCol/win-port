/* PORT: this file's own declarations of num_cargo_vessels removed: the header declares it from the definition (W1 step 3). */
/* compute_europe_parameters: candidate for 1048:4ca4 (40 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: compute_europe_parameters
 *
 * Recounts the ships in port into DGROUP:0x1fca and the damaged ones among them into 0x1fcc,
 * clears the two words at 0x4ce2/0x4cde and runs reset_europe_units.
 * Both helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from reset_chain_counts: it recounts the Europe dock: the ships in port and the damaged ones among them (num_cargo_vessels), clears the two ship selections, and recounts the land units.
 *
 * RENAMED 2026-09-26 from europe_recount_dock: the Mac build's compute_europe_parameters(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller DrawEurope; callee reset_europe_units), the runner-up gets 1, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int g_damaged_in_port;                   /* DGROUP:0x1fcc, the damaged ships in port, returned through num_cargo_vessels's pointer */
extern int g_ships_in_port;                  /* DGROUP:0x1fca */

extern int g_stub_a;

extern void reset_europe_units(void);

void compute_europe_parameters(void)
{
    g_ships_in_port = num_cargo_vessels(&g_damaged_in_port);
    g_europe_selected_ship = g_europe_ship_highlight = 0;
    reset_europe_units();
}
