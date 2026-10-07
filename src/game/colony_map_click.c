/* colony_map_click: candidate for 1010:4354 (484 bytes).  Verdict: RELOCATABLE.
 * symbol: colony_map_click
 * name-source: sibling-build
 *
 * A click on one of the colony's nine work squares, which the window has already reduced to a
 * (column, row) pair at DGROUP:0xc08/0xc0a. A marked cell -- `g_grid_marks`, which
 * `colony_indian_table` sets for squares the natives claim -- does nothing.
 *
 * An EMPTY square takes the selected colonist, if the job will have them: the colonist's
 * current job is asked for, and jobs 0 and 8 (farmer, fisherman) and every indoor job from 9 up
 * are replaced by the square's own: job 8 on water and job 0 on land. Jobs 1..7, the other
 * outdoor jobs, ask with themselves. `try_to_set_job` answers, and a zero answer -- read
 * through the corpus's `NEG / SBB / INC` negation -- is what lets the move happen.
 *
 * An OCCUPIED square does not take anyone. Clicking the colonist already there deselects
 * (`colony_select_profession(0)`); clicking a different one SELECTS them, which is how the
 * player picks up a colonist from the field.
 *
 * The square's map position is the colony's own plus the cell minus two, so (2,2) is the
 * colony itself and the grid is the eight squares around it.
 *
 * `(int)get_field(...) == -1` needs its cast: without it Borland compares the
 * returned byte in place where the shipped `CBW`s it first (rule 51). The second comparison
 * goes the other way -- `cmp al,[es:0x4bbc]` with no widening either side -- so that one is
 * written `(char)g_selected_colonist`.
 *
 * RENAMED 2026-09-26 from colony_assign_cell: the Mac build's colony_map_click(void), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller ColonyMouseMap; callee colony_change_selected; callee colony_select_profession; callee get_field; callee get_job), the runner-up gets 5, and the arity agrees (0).
 */

#include "colonization.h"

#include "jobs.h"
#include "records.h"

extern char get_field(int col, int row);
extern void arm_indians(int col, int row);
extern int get_job(int colonist);
extern int terrain_is_ocean(int x, int y);
extern int try_to_set_job(int c, int job);
extern void pick_up_colonist(int id);
extern void set_field(int col, int row, int colonist);
extern void colony_select_profession(int unit_roles_only);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void colony_change_selected(void);

void colony_map_click(void)
{
    int tx;
    int ty;
    register int job;
    register int ok;

    if (g_grid_marks[g_cell_col][g_cell_row] == 0) {
        if ((int)get_field(g_cell_col, g_cell_row) == -1) {
            arm_indians(g_cell_col, g_cell_row);
            job = get_job(g_selected_colonist);
            if (job >= JOB_INDOOR_FIRST || job == JOB_FARMER || job == JOB_FISHERMAN) {
                tx = g_cur_colony_rec->x + g_cell_col - 2;
                ty = g_cur_colony_rec->y + g_cell_row - 2;
                if (terrain_is_ocean(tx, ty) != 0)
                    ok = !try_to_set_job(g_selected_colonist, 8);
                else
                    ok = !try_to_set_job(g_selected_colonist, 0);
            } else {
                ok = !try_to_set_job(g_selected_colonist, job);
            }
            if (ok != 0) {
                pick_up_colonist(g_selected_colonist);
                set_field(g_cell_col, g_cell_row, g_selected_colonist);
            }
        } else {
            if (get_field(g_cell_col, g_cell_row)
                    != (char)g_selected_colonist)
                g_selected_colonist = get_field(g_cell_col, g_cell_row);
            else
                colony_select_profession(0);
        }
    }
    colony_change_selected();
}
