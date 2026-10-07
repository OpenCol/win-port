/* job_change_legal: candidate for 1018:8637 (541 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 541, not the census 532. The trailing 9 bytes are the dead second epilogue.
 *
 * THE ADVISOR'S OBJECTION to putting colonist `k` on `job`, as a code, or 0 for none. The job the
 * colonist already has answers 0 at once.
 *
 * Moving OUT of the colony (`job_transfer_type` says 2) has three: 0x15 when the
 * colony is at most 3 strong and flag 0 is set, 0x14 when the job is neither 0x15 nor 0x17 and
 * `siege_check(0, 0, 0)` objects, and 3 when the colony would be emptied.
 * Move kind 3 into a colony of 32 gives 4; job 8 without colony flag 6 gives 0xd.
 *
 * JOB 0x12 HAS ITS OWN LADDER. How many may hold it depends on which of colony flags 0xe, 0xd
 * and 0xc is set -- three, two, one -- and passing that, the colonist's speciality category (+4
 * of the 8-byte speciality record, with 0x1c read as 0x19) must be no more than 3, and 3 or 2
 * need their flag. Codes 7, 8, 9, 0x10, 0x12, 0x11.
 *
 * Last, the colony's other colonists are tallied by job into a 25-entry local and three or more
 * already on a job above 9 gives 0x16.
 *
 * That last `if` ends in a `goto out` of its own -- an `EB 00` onto the next instruction, which
 * is the only thing separating it from the same `if` without one (rule 54). The skip in the
 * tally loop is a `continue` in the two-jump form (rule 12), and six module helpers come through
 * the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from job_warning: the Mac build's job_change_legal(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller try_to_set_job; callee get_job; callee get_specialty; callee has_building), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "jobs.h"
#include "building.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern int get_job(int colonist);
extern int get_specialty(int u);
extern int job_transfer_type(int colonist, int job);
extern int has_building(int bit);
extern int siege_check(long best_out, long own_out, long total_out);
extern int job_count(int job);

int job_change_legal(int colonist, int job)
{
    int kind;
    int cur;
    int n;
    int spec;
    int j;
    int counts[25];
    register int code;
    register int i;

    code = 0;
    cur = get_job(colonist);
    spec = get_specialty(colonist);
    if (job == cur)
        goto out;
    kind = job_transfer_type(colonist, job);
    if (kind == 2) {
        if (has_building(BUILDING_STOCKADE) != 0 && (int)g_cur_colony_rec->pop <= 3) {
            code = 0x15;
            goto out;
        }
        if (job != JOB_SOLDIER && job != JOB_DRAGOON
            && siege_check(0L, 0L, 0L) != 0) {
            code = 0x14;
            goto out;
        }
        if ((int)g_cur_colony_rec->pop == 1) {
            code = 3;
            goto out;
        }
    }
    if (kind == 3 && (int)g_cur_colony_rec->pop >= 0x20) {
        code = 4;
        goto out;
    }
    if (job == JOB_FISHERMAN && has_building(BUILDING_DOCKS) == 0) {
        code = 0xd;
        goto out;
    }
    if (job == JOB_TEACHER) {
        n = job_count(0x12);
        if (has_building(BUILDING_UNIVERSITY) != 0) {
            if (n >= 3) {
                code = 7;
                goto out;
            }
        } else if (has_building(BUILDING_COLLEGE) != 0) {
            if (n >= 2) {
                code = 8;
                goto out;
            }
        } else if (has_building(BUILDING_SCHOOLHOUSE) != 0) {
            if (n >= 1) {
                code = 9;
                goto out;
            }
        }
        if (spec == JOB_COLONIST_ALT)
            spec = 0x19;
        if (g_spec_recs[spec].school > SCHOOL_UNIVERSITY) {
            code = 0x10;
            goto out;
        }
        if (g_spec_recs[spec].school == SCHOOL_UNIVERSITY && has_building(BUILDING_UNIVERSITY) == 0) {
            code = 0x12;
            goto out;
        }
        if (g_spec_recs[spec].school == SCHOOL_COLLEGE && has_building(BUILDING_COLLEGE) == 0) {
            code = 0x11;
            goto out;
        }
    }
    memset_rtl(counts, 0, 0x32);
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        if (i == colonist)
            continue;
        j = get_job(i);
        counts[j]++;
    }
    if (counts[job] >= 3 && job > 9) {
        code = 0x16;
        goto out;
    }
out:
    return code;
}
