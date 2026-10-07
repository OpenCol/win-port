/* job_index: candidate for 1018:3bd6 (31 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Maps speciality 0x1c onto 0x13 and passes everything else through -- the
 * same fold `get_icon` makes inline before its lookup.
 *
 * RENAMED 2026-09-26 from fold_spec_1c: folds JOB_COLONIST_ALT (0x1c) onto JOB_COLONIST and passes every other
 * job through -- the portrait fold get_icon makes inline.
 *
 * RENAMED 2026-09-26 from fold_colonist_alt: the Mac build's job_index(short), bound by call-graph agreement (2 of its Mac call edges already bound land here, runner-up 0) and by call-sequence alignment (the lone unbound callee of its gap in job_desc, job_name) and by layout (on both sides between find_close_colony and job_name); the arity agrees and the Mac body is 1.16x this one. Its body folds the alternate colonist specialty (0x1c) onto the plain one: a job index.
 */

#include "colonization.h"

#include "jobs.h"
int job_index(int spec)
{
    if (spec == JOB_COLONIST_ALT)
        spec = JOB_COLONIST;
    return spec;
}
