/* specialty_count: candidate for 1018:59c7 (60 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: specialty_count
 *
 * How many colonists of the current colony have speciality `spec`.
 * get_specialty is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from colony_count_specialty: the Mac build's specialty_count(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller colony_plan; callee get_specialty), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int get_specialty(int u);

int specialty_count(int spec)
{
    register int n = 0;
    register int colonist;

    for (colonist = 0; colonist < g_cur_colony_rec->pop; colonist++) {
        if (get_specialty(colonist) == spec)
            n++;
    }
    return n;
}
