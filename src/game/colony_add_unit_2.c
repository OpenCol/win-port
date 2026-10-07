/* PORT: this file's own declarations of set_job removed: the header declares it from the definition (W1 step 3). */
/* colony_add_unit_2: candidate for 1018:aeaf (73 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_add_unit_2
 *
 * Makes unit u a colonist of colony `colony`: selects the colony, refreshes
 * it (colony_indian_table, compute_colony_parameters), finds the unit's colonist index and
 * gives it job 0 through set_job.  Returns an uninitialised frame
 * slot, as the shipped code does.  All five helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_take_unit: the Mac build's colony_add_unit_2(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller plan_move; callee colony_indian_table; callee set_colony; callee set_job), the runner-up gets 2, and the arity agrees (2). Its sibling colony_add_unit is colony_employ_unit: the Mac colony_add_unit_2 calls set_job as this does, colony_add_unit calls try_to_set_job as that does.
 */

#include "colonization.h"

extern int g_stub_a;

extern void set_colony(int colony);
extern void colony_indian_table(void);
extern void compute_colony_parameters(void);
extern int guy_number_of_unit(int u);

int colony_add_unit_2(int colony, int u)
{
    int colonist;
    int result;

    set_colony(colony);
    colony_indian_table();
    compute_colony_parameters();
    colonist = guy_number_of_unit(u);
    set_job(colonist, 0);
    return result;
}
