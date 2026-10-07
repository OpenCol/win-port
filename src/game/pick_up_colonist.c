/* pick_up_colonist: candidate for 1018:62db (59 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: pick_up_colonist
 *
 * Clears the colony grid cell a colonist occupies, when that colonist has one.
 *
 * RENAMED 2026-09-26 from clear_colonist_cell: the Mac build's pick_up_colonist(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller colony_plan; caller colony_select_profession; caller try_to_set_job), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

extern int colonist_find(int id, int *px, int *py);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_field(int col, int row, int colonist);
void pick_up_colonist(int id)
{
    int col;
    int row;

    if (colonist_find(id, &col, &row))
        set_field(col, row, -1);
}
