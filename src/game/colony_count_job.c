/* colony_count_job -- shipped at 1018:52e8.
 *
 * Ported by tools/port.py from matched/game/colony_count_job.c.
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

/* 0xca bytes */





col_i16 colony_count_job(col_i16 job)
{
    register col_i16 n = 0;
    register col_i16 c;

    for (c = 0; c < g_cur_colony_rec->pop; c++) {
        if (colonist_job(c) == job)
            n++;
    }
    return n;
}
