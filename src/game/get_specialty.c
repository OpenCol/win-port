/* get_specialty: candidate for 1018:4c6a (97 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Specialty of colonist i of the current colony (see set_specialty); the colony
 * byte reads unsigned, the unit byte signed.
 *
 * RENAMED 2026-09-26 from colonist_specialty_get: the Mac build's get_specialty(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller ReportsLabor; caller Score; caller colony_select_profession; caller combat_fight; caller new_turn_colony), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int nth_unit_with_job(int k);
int get_specialty(int u)
{
    int r;

    if (g_cur_colony_rec->pop > u)
        r = g_cur_colony_rec->spec[u];
    else
        r = g_units[nth_unit_with_job(u - g_cur_colony_rec->pop)].spec;
    return r;
}
