/* colonist_portrait -- shipped at 1018:4d71.
 *
 * Ported by tools/port.py from matched/game/colonist_portrait.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colonist_job(col_u16 c);
col_i16 colonist_specialty_get(col_i16 k);
col_i16 nth_colonist_unit(col_i16 n);

#include "units.h"
#include "jobs.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

/* 0xca bytes */


extern col_i16 specialist_portrait(col_i16 spec);





col_i16 colonist_portrait(col_i16 c)
{
    col_i16 job;
    col_i16 r;
    col_i16 spec;
    col_i16 match;

    job = colonist_job(c);
    spec = colonist_specialty_get(c);
    if (job <= JOB_COLONIST) {
        if (spec == JOB_COLONIST_ALT)
            spec = JOB_COLONIST;
        r = specialist_portrait(spec);
    } else {
        r = job + 0x52;
        match = spec == job;
        if (spec == 0x15 && job == JOB_DRAGOON)
            match = 1;
        if (match == 0)
            r = job + 0x36;
        if (job == JOB_SOLDIER || job == JOB_DRAGOON) {
            c = nth_colonist_unit(c - g_cur_colony_rec->pop);
            if (g_units[c].type == UNIT_CONTINENTAL_ARMY || g_units[c].type == UNIT_CONTINENTAL_CAVALRY)
                r = g_unit_types[g_units[c].type].icon;
        }
    }
    return r;
}
