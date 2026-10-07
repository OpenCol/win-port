/* guys_at_building: candidate for 1018:5e0d (126 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: guys_at_building
 *
 * How many colonists work in a building. For a building whose base is 0 the
 * negated count of colonist units on the colony tile; otherwise the number of
 * colonists holding the job worked there (job_of_building), or 0 when
 * that job is negative. The argument is a building and was named `job`, and the job
 * was named `goods`, until 2026-09-27.  All four helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from job_workers: the Mac build's guys_at_building(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DrawABuilding; callee is_colonist; callee job_count; callee next_unit), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern int next_unit(int u);

extern int base_building(int building);
extern int is_colonist(int u);
extern int job_of_building(int building);
extern int job_count(int job);

int guys_at_building(int building)
{
    register int u;
    register int n = 0;
    int job;

    if (base_building(building) == 0) {
        for (u = g_tile_first_unit; u >= 0; u = next_unit(u)) {
            if (is_colonist(u) != 0)
                n++;
        }
        n = 0 - n;
    } else {
        job = job_of_building(building);
        if (job < 0)
            goto done;
        n = job_count(job);
    }
done:
    return n;
}
