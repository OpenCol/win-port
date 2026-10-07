/* colony_add_unit: candidate for 1018:ae62 (77 bytes).  Verdict: RELOCATABLE.
 * symbol: colony_add_unit
 * name-source: sibling-build
 *
 * Puts unit `u` to work in colony `colony` as job 0: selects the colony, recomputes the tribes' claims
 * on its squares (`colony_indian_table`) and 1018:8c7c, and hands the unit's colonist index
 * to `try_to_set_job` -- so the job objections and the native land prompt apply. Returns
 * that function's answer, 0 when the colonist went to work.
 *
 * It was `colony_bribe_unit_1018_ae62`, named after its callee's placeholder `txt_INDIANBRIBE_1010_c182`,
 * the one message among twenty-four that the placeholder happened to pick. The four helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_employ_unit: the Mac build's colony_add_unit(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller orders_build; callee colony_indian_table; callee set_colony; callee try_to_set_job), the runner-up gets 2, and the arity agrees (2). Its sibling colony_add_unit_2 is colony_take_unit: the Mac colony_add_unit calls try_to_set_job as this does, colony_add_unit_2 calls set_job as that does.
 */

#include "colonization.h"

extern int g_stub_a;
extern int try_to_set_job(int c, int a);

extern void set_colony(int colony);
extern void colony_indian_table(void);
extern void compute_colony_parameters(void);
extern int guy_number_of_unit(int u);

int colony_add_unit(int colony, int u)
{
    int colonist;
    int result;

    set_colony(colony);
    colony_indian_table();
    compute_colony_parameters();
    colonist = guy_number_of_unit(u);
    result = try_to_set_job(colonist, 0);
    return result;
}
