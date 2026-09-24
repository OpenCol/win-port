/* job_workers -- shipped at 1018:5e0d.
 *
 * Ported by tools/port.py from matched/game/job_workers.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_tile_first_unit;                        DGROUP:0x4bb8
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_family_base(col_i16 job);
col_i16 colony_count_job(col_i16 job);
col_i16 job_goods(col_i16 job);
col_i16 unit_is_colonist(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */



extern col_i16 unit_chain_next(col_i16 u);






col_i16 job_workers(col_i16 job)
{
    register col_i16 u;
    register col_i16 n = 0;
    col_i16 t;

    if (building_family_base(job) == 0) {
        for (u = g_tile_first_unit; u >= 0; u = unit_chain_next(u)) {
            if (unit_is_colonist(u) != 0)
                n++;
        }
        n = 0 - n;
    } else {
        t = job_goods(job);
        if (t < 0)
            goto done;
        n = colony_count_job(t);
    }
done:
    return n;
}
