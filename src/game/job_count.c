/* job_count: candidate for 1018:52e8 (60 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: job_count
 *
 * How many colonists of the current colony hold job `job`.  get_job is
 * -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from colony_count_job: the Mac build's job_count(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller DrawIndividual; caller colony_plan; caller job_change_legal; caller new_turn_colony; callee get_job), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int get_job(int colonist);

int job_count(int job)
{
    register int n = 0;
    register int colonist;

    for (colonist = 0; colonist < g_cur_colony_rec->pop; colonist++) {
        if (get_job(colonist) == job)
            n++;
    }
    return n;
}
