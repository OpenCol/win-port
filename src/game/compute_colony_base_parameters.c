/* compute_colony_base_parameters: candidate for 1018:8c35 (49 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Rebuilds everything the colony screen works from: the grid marks, the marked cells, the
 * idle colonists placed, the colonists sorted, and the list of cargo-carrying units --
 * whose count lands at DGROUP:0xc14 and whose selection at 0xc16 is reset, along with
 * 0xc18. `ColonyWin_ColonyMouseShipsCargo` and `colony_click_ship` both read 0xc16 as the
 * selected carrier.
 *
 * `compute_colony_dynamic` is the next function along and the light version: two
 * calls where this is five.
 *
 * Every callee is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from colony_rebuild_screen: Mac compute_colony_base_parameters(void), 84 bytes, against 49. Caller: compute_colony_parameters on both. Callees: colony_field_table, pick_up_invalid_colonists, preassign_colonists_to_fields, sort_colonists_by_profession and num_cargo_vessels(void), the same five on both (72e4, 7a42, 604c, 7af0). Layout: Mac eligible_to_build, count_eligible_builds, nth_eligible_build, [this], compute_colony_dynamic, compute_colony_parameters, compute_colony_screen; Windows the same with 8c35 in the slot. Arity: void.
 */

#include "colonization.h"

extern int g_stub_a, g_stub_b, g_stub_c, g_stub_d;

/* same object: 4-byte `push cs; call ` */
extern void colony_field_table(void);
extern void pick_up_invalid_colonists(void);
extern void preassign_colonists_to_fields(void);
extern void sort_colonists_by_profession(void);

void compute_colony_base_parameters(void)
{
    colony_field_table();
    pick_up_invalid_colonists();
    preassign_colonists_to_fields();
    sort_colonists_by_profession();
    g_carrier_count = colony_cargo_units();
    g_selected_carrier = 0;
    g_carrier_highlight = 0;
}
