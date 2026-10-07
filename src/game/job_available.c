/* job_available: candidate for 1018:847c (443 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 443, not the census 434. The trailing 9 bytes are the dead second epilogue.
 *
 * A VERDICT ON JOB `job` FOR THE SELECTED COLONIST IN THE CURRENT COLONY, as -2, -1 or 0. It
 * starts at -2; the tests below either return 0 or leave it, and a job whose input goods are
 * short sets -1. `colony_plan` stores one verdict per job 0..0x12 for the colony screen.
 *
 * Jobs below 0x13 answer 0 when their building slot (`building_of_job`) exists with
 * its colony flag clear. Job 0x12 also answers 0 by the colonist's speciality category -- +4
 * of the 8-byte speciality record, speciality 0x1c read as 0x19: category 4 always, category
 * 3 unless colony flag 0xe is set, category 2 unless flag 0xd is.
 *
 * From 0x13 up: speciality 0x1b answers 0 for any job above 0x13; job 0x13 answers 0 only for
 * speciality 0x18 in a colony that passes the human test below; job 0x18 answers 0 when colony
 * flag 0x25 is clear and the colonist is NOT speciality 0x18. Every other building job keeps
 * -2 unless an input goods (`job_cargo`) is short -- 20 of goods 0xe, none of
 * goods 0, 50 of anything else -- which makes it -1.
 *
 * THE HUMAN-COLONY TEST LOOKS LIKE A SHIPPED BUG. It indexes the 0x34-byte NATION records at
 * SEG20:0x7841 with DGROUP:0x4ba2 -- which `set_colony` fills with the colony's INDEX --
 * after checking that index is below 4. So only colonies 0..3 can pass it, and they pass by
 * their index's nation, not their owner's.
 *
 * The goods switch has four cases and a default and Borland lays its test out as a tree: 0xe
 * first, then `JG` to 0xf and the rest by equality. Case 0xf shares the default's body, and
 * that body's `break` is the `EB 00` of a last case (rule 54). Four helpers come through the
 * 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from job_verdict: the Mac build's job_available(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_plan; caller colony_select_profession; callee get_specialty; callee has_building), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "nations.h"
#include "jobs.h"
#include "control.h"
#include "building.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

/* same object: 4-byte `push cs; call ` */
extern int building_of_job(int job);
extern int has_building(int bit);
extern int get_specialty(int u);
extern int job_cargo(int *goods, int job);

int job_available(int job)
{
    int r;
    int slot;
    int n;
    int spec;
    int kind;
    int inputs[3];
    register int i;
    register int need;

    r = -2;
    if (job < 0x13) {
        slot = building_of_job(job);
        if (slot >= 0 && has_building(slot) == 0)
            r = 0;
        if (job == JOB_TEACHER) {
            spec = get_specialty(g_selected_colonist);
            if (spec == JOB_COLONIST_ALT)
                spec = JOB_INDENTURED_SERVANT;
            kind = g_spec_recs[spec].school;
            if (kind == SCHOOL_NONE)
                r = 0;
            if (kind == SCHOOL_UNIVERSITY && has_building(BUILDING_UNIVERSITY) == 0)
                r = 0;
            if (kind == SCHOOL_COLLEGE && has_building(BUILDING_COLLEGE) == 0)
                r = 0;
        }
    } else {
        n = job_cargo(inputs, job);
        spec = get_specialty(g_selected_colonist);
        if (job > 0x13 && spec == JOB_CONVERT) {
            r = 0;
            goto out;
        }
        switch (job) {
        case 0x13:
            if (spec == JOB_MISSIONARY && g_cur_colony_idx < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_colony_idx].control == CONTROL_HUMAN) {
                r = 0;
                goto out;
            }
            break;
        case 0x18:
            if (has_building(BUILDING_CHURCH) == 0 && spec != JOB_MISSIONARY) {
                r = 0;
                goto out;
            }
            break;
        }
        if (n >= 0) {
            for (i = 0; i < n; i++) {
                switch (inputs[i]) {
                case 0xe:
                    need = 0x14;
                    break;
                case 8:
                    need = 0x32;
                    break;
                case 0:
                    need = 0;
                    break;
                case 0xf:
                default:
                    need = 0x32;
                    break;
                }
                if (g_cur_colony_rec->stock[inputs[i]] < need)
                    r = -1;
            }
        }
    }
out:
    return r;
}
