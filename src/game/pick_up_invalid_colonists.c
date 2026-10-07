/* pick_up_invalid_colonists: candidate for 1018:72e4 (67 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: pick_up_invalid_colonists
 *
 * For every marked cell of the 5x5 table at SEG21:0x14a calls
 * set_field(i, j, -1).  The helper is -called and defined
 * in-file as a stub.
 *
 * RENAMED 2026-09-27 from clear_marked_cells: Mac pick_up_invalid_colonists(void), 104 bytes, against 67. Caller: compute_colony_base_parameters (8c35) on both. Callee: set_field only. Arity: void. Layout: the Mac CODE 7 run colony_indian_table, pick_up_invalid_colonists, preassign_colonist_to_field, preassign_colonists_to_fields, num_cargo_vessels(void), nth_cargo_vessel is the Windows 1018 run colony_indian_table, 72e4, 7327, 7a42, 7af0, nth_cargo_vessel, one for one between bound neighbours.
 */

#include "colonization.h"

extern int g_stub_a;

extern void set_field(int col, int row, int colonist);

void pick_up_invalid_colonists(void)
{
    register int j;
    register int i;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            if (g_grid_marks[i][j] != 0)
                set_field(i, j, -1);
        }
    }
}
