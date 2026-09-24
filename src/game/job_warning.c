/* job_warning -- shipped at 1018:8637.
 *
 * Ported by tools/port.py from matched/game/job_warning.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *
 * The file's own Colony, SpecRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colonist_job(col_i16 c);
col_i16 colonist_move_kind(col_i16 c, col_i16 job);
col_i16 colonist_specialty_get(col_i16 c);
col_i16 colony_count_job(col_i16 job);
col_i16 colony_trade_summary(col_i32 a, col_i32 b, col_i32 c);
col_i16 current_colony_flag_get(col_i16 bit);

#include "jobs.h"
#include "building.h"
/* 8 bytes at SEG20:0xd61a */


extern void memset_rtl(void *p, col_i16 c, col_i16 n);

/* same object: 4-byte `push cs; call ` */







col_i16 job_warning(col_i16 k, col_i16 job)
{
    col_i16 kind;
    col_i16 cur;
    col_i16 n;
    col_i16 spec;
    col_i16 j;
    col_i16 counts[25];
    register col_i16 code;
    register col_i16 i;

    code = 0;
    cur = colonist_job(k);
    spec = colonist_specialty_get(k);
    if (job == cur)
        goto out;
    kind = colonist_move_kind(k, job);
    if (kind == 2) {
        if (current_colony_flag_get(BUILDING_STOCKADE) != 0 && (col_i16)g_cur_colony_rec->pop <= 3) {
            code = 0x15;
            goto out;
        }
        if (job != JOB_SOLDIER && job != JOB_DRAGOON
            && colony_trade_summary(0L, 0L, 0L) != 0) {
            code = 0x14;
            goto out;
        }
        if ((col_i16)g_cur_colony_rec->pop == 1) {
            code = 3;
            goto out;
        }
    }
    if (kind == 3 && (col_i16)g_cur_colony_rec->pop >= 0x20) {
        code = 4;
        goto out;
    }
    if (job == JOB_FISHERMAN && current_colony_flag_get(BUILDING_DOCKS) == 0) {
        code = 0xd;
        goto out;
    }
    if (job == JOB_TEACHER) {
        n = colony_count_job(0x12);
        if (current_colony_flag_get(BUILDING_UNIVERSITY) != 0) {
            if (n >= 3) {
                code = 7;
                goto out;
            }
        } else if (current_colony_flag_get(BUILDING_COLLEGE) != 0) {
            if (n >= 2) {
                code = 8;
                goto out;
            }
        } else if (current_colony_flag_get(BUILDING_SCHOOLHOUSE) != 0) {
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
        if (g_spec_recs[spec].school == SCHOOL_UNIVERSITY && current_colony_flag_get(BUILDING_UNIVERSITY) == 0) {
            code = 0x12;
            goto out;
        }
        if (g_spec_recs[spec].school == SCHOOL_COLLEGE && current_colony_flag_get(BUILDING_COLLEGE) == 0) {
            code = 0x11;
            goto out;
        }
    }
    memset_rtl(counts, 0, 0x32);
    for (i = 0; (col_i16)g_cur_colony_rec->pop > i; i++) {
        if (i == k)
            continue;
        j = colonist_job(i);
        counts[j]++;
    }
    if (counts[job] >= 3 && job > 9) {
        code = 0x16;
        goto out;
    }
out:
    return code;
}
