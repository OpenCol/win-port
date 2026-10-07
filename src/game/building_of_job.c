/* building_of_job: candidate for 1018:47af (46 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The building a job works in: the signed byte at SEG21:0xf7 for a job under 0x13
 * (JOB_COLONIST); -1 otherwise. The table is indexed by JOB -- every caller passes one
 * (DoJobArticle, compute_colonist_yield, job_available) -- and job_of_building (SEG21:0xcd)
 * is its inverse.
 *
 * RENAMED 2026-09-26 from building_slot_of: the Mac build's building_of_job(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DoJobArticle; caller compute_colonist_yield; caller job_available), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

int building_of_job(int job)
{
    int r = -1;

    if (job < 0x13)
        r = g_building_of_job[job];
    return r;
}
