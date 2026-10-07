/* job_verdict -- shipped at 1018:847c.
 *
 * Ported by tools/port.py from matched/game/job_verdict.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_cur_colony_idx;                         DGROUP:0x4ba2
 *   extern int g_selected_colonist;                      DGROUP:0x4bbc
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *
 * The file's own Colony, SpecRec, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_slot_of(col_i16 job);
col_i16 colonist_specialty_get(col_i16 k);
col_i16 current_colony_flag_get(col_i16 bit);
col_i16 job_input_goods(void *goods, col_i16 job);

#include "nations.h"
#include "jobs.h"
#include "control.h"
#include "building.h"
/* 8 bytes at SEG20:0xd61a */
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */






/* same object: 4-byte `push cs; call ` */





col_i16 job_verdict(col_i16 job)
{
    col_i16 r;
    col_i16 slot;
    col_i16 n;
    col_i16 spec;
    col_i16 kind;
    col_i16 inputs[3];
    register col_i16 i;
    register col_i16 need;

    r = -2;
    if (job < 0x13) {
        slot = building_slot_of(job);
        if (slot >= 0 && current_colony_flag_get(slot) == 0)
            r = 0;
        if (job == JOB_TEACHER) {
            spec = colonist_specialty_get(g_selected_colonist);
            if (spec == JOB_COLONIST_ALT)
                spec = JOB_INDENTURED_SERVANT;
            kind = g_spec_recs[spec].school;
            if (kind == SCHOOL_NONE)
                r = 0;
            if (kind == SCHOOL_UNIVERSITY && current_colony_flag_get(BUILDING_UNIVERSITY) == 0)
                r = 0;
            if (kind == SCHOOL_COLLEGE && current_colony_flag_get(BUILDING_COLLEGE) == 0)
                r = 0;
        }
    } else {
        n = job_input_goods(inputs, job);
        spec = colonist_specialty_get(g_selected_colonist);
        if (job > 0x13 && spec == JOB_CONVERT) {
            r = 0;
            goto out;
        }
        switch (job) {
        case 0x13:
            if (spec == JOB_MISSIONARY && g_cur_colony_idx < NATION_EUROPEAN_COUNT && g_nation52[g_cur_colony_idx].control == CONTROL_HUMAN) {
                r = 0;
                goto out;
            }
            break;
        case 0x18:
            if (current_colony_flag_get(BUILDING_CHURCH) == 0 && spec != JOB_MISSIONARY) {
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
                if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[inputs[i]] < need)
                    r = -1;
            }
        }
    }
out:
    return r;
}
