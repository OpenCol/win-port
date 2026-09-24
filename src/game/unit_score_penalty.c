/* unit_score_penalty -- shipped at 1028:08d3.
 *
 * Ported by tools/port.py from matched/game/unit_score_penalty.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern unsigned char far g_nation_colony_count[4];   DGROUP:0x4b48
 *   extern int far g_nearest_dist;                       DGROUP:0x4bc6
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *
 * The file's own Unit, Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1028_e376(col_i16 a, col_i16 b);
col_i16 nation_score_base(col_i16 nation);

#include "jobs.h"
#include "units.h"
/* 0x13c bytes */





extern col_i16 find_nearest_colony(col_i16 x, col_i16 y, col_i16 nation, col_i16 land);
extern col_i16 is_ordinary_job(col_i16 job);

/* same object: 4-byte `push cs; call ` */



col_i16 unit_score_penalty(col_i16 nation, register col_i16 u, col_i16 land)
{
    register col_i16 score = 0;

    if (g_nation_colony_count[nation] != 0) {
        if (find_nearest_colony(g_units[u].x, g_units[u].y, nation, land) >= 0)
            score += g_nearest_dist / 5 - 1;
        else
            score = 2;
    }
    if (g_units[u].type == UNIT_PIONEERS)
        score += 2;
    if (g_units[u].type == UNIT_SOLDIERS)
        score -= 2;
    if (g_units[u].type == UNIT_DRAGOONS)
        score -= 3;
    if (g_units[u].type == UNIT_COLONISTS) {
        score -= 2;
        if (is_ordinary_job(g_units[u].spec) != 0)
            score -= 2;
        if ((col_i16)g_units[u].spec == JOB_CONVERT)
            score -= 20;
    }
    if (nation_score_base(nation) != 0)
        score += (g_turn_counter - g_nations[nation].founded) >> 4;
    score = min_int_1028_e376(0, score);
    return score;
}
