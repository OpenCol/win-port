/* pick_recruit -- shipped at 1048:6c51.
 *
 * Ported by tools/port.py from matched/game/pick_recruit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Nation, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "jobs.h"
/* 316 bytes at SEG20:0xc2f4 */
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */




extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
extern col_i16 is_ordinary_job(col_i16 job);
extern col_i16 lfsr_step(col_i16 *p, col_i16 k);

unsigned char pick_recruit(col_i16 from_pool)
{
    col_i16 r;
    col_i16 found;
    col_i16 tries;
    register col_i16 i;
    register col_i16 d;

    tries = 0;
    if (from_pool != 0)
        goto pool;
    d = g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN ? g_difficulty : 1;
    d++;
    if (rand_range(1, 15) <= (d + 2) >> 1) {
        r = nation_flag_test(g_cur_nation, FATHER_WILLIAM_BREWSTER) ? JOB_COLONIST_ALT : JOB_CRIMINAL;
    } else if (rand_range(1, 10) <= (d + 2) >> 1) {
        r = nation_flag_test(g_cur_nation, FATHER_WILLIAM_BREWSTER) ? JOB_COLONIST_ALT : JOB_INDENTURED_SERVANT;
    } else if (rand_range(1, 8) <= (d + 2) >> 1) {
        r = JOB_COLONIST_ALT;
    } else {
pool:
        found = 0;
        for (i = 0; i < 3; i++)
            if (is_ordinary_job(((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[i]) == 0)
                found = 1;
        if (found == 0) {
            r = JOB_COLONIST_ALT;
        } else {
            do {
                r = g_cur_nation_rec->seed;
                g_cur_nation_rec->seed = lfsr_step(&r, 0x14);
                r = (g_cur_nation_rec->seed + g_cur_nation_rec->seed_add) & 0x1f;
            } while (r < 0 || r >= JOB_INDENTURED_SERVANT);
            if (r == JOB_TEACHER)
                r = JOB_CARPENTER;
            if (r == JOB_COLONIST)
                r = JOB_SCOUT;
            if (r == JOB_SUGAR_PLANTER)
                r = JOB_FISHERMAN;
            if (r == JOB_TOBACCO_PLANTER)
                r = JOB_LUMBERJACK;
            if (r == JOB_COTTON_PLANTER)
                r = JOB_FARMER;
            if (r == JOB_FUR_TRAPPER)
                r = JOB_ORE_MINER;
            if (r == JOB_DRAGOON)
                r = JOB_CARPENTER;
            tries++;
            if (tries > 100)
                goto done;
            for (i = 0; i < 3; i++)
                if (((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[i] == r)
                    goto pool;
        }
    }
done:
    return r;
}
