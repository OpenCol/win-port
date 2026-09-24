/* experts_at_work -- shipped at 1018:5a03.
 *
 * Ported by tools/port.py from matched/game/experts_at_work.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colonist_job(col_i16 c);
col_i16 colonist_specialty_get(col_i16 k);

#include "jobs.h"
/* 0xca bytes */






col_i16 experts_at_work(void)
{
    register col_i16 c;
    register col_i16 spec;
    col_i16 job;
    col_i16 n = 0;

    for (c = 0; c < g_cur_colony_rec->pop; c++) {
        spec = colonist_specialty_get(c);
        job = colonist_job(c);
        if (spec < JOB_COLONIST && spec == job)
            n++;
    }
    return n;
}
