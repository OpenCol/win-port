/* get_job: candidate for 1018:4c11 (89 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_job
 *
 * The job byte of colonist c: for c below the colony's population it is
 * Colony+0x20[c]; past it, c counts the colonist units on the tile
 * (nth_unit_with_job) and the answer is that unit type's colonist class.
 * Both helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colonist_job: the Mac build's get_job(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller DrawColonyMap; caller colony_select_profession; caller new_turn_colony; caller resolve_raid), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int nth_unit_with_job(int n);
extern int job_of_unit(int u);

int get_job(int colonist)
{
    int r;

    if (g_cur_colony_rec->pop > colonist)
        r = g_cur_colony_rec->jobs[colonist];
    else
        r = job_of_unit(nth_unit_with_job(colonist - g_cur_colony_rec->pop));
    return r;
}
