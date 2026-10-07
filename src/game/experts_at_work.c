/* experts_at_work: candidate for 1018:5a03 (93 bytes).  Verdict: RELOCATABLE.
 * symbol: experts_at_work
 *
 * How many colonists of the current colony hold a building or field job
 * (< 0x13) that equals their speciality.  Both helpers are -called and
 * defined in-file as stubs.
 */

#include "colonization.h"

#include "jobs.h"
#include "records.h"

extern int g_stub_a;

extern int get_specialty(int u);
extern int get_job(int colonist);

int experts_at_work(void)
{
    register int colonist;
    register int spec;
    int job;
    int n = 0;

    for (colonist = 0; colonist < g_cur_colony_rec->pop; colonist++) {
        spec = get_specialty(colonist);
        job = get_job(colonist);
        if (spec < JOB_COLONIST && spec == job)
            n++;
    }
    return n;
}
