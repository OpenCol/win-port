/* europe_init_nations -- shipped at 1048:81c1.
 *
 * Ported by tools/port.py from matched/game/europe_init_nations.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern GoodsRec far g_goods[];                       SEG20:0xd79c
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_random_seed;                        DGROUP:0x4b86
 *
 * The file's own Nation, GoodsRec, NationRec52 dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: horses -> horses_stock, muskets -> f49, royal_gold -> f22, tories -> f18
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char pick_recruit(col_i16 from_pool);
void set_current_nation(col_i16 n);
void update_market_price(col_i16 a, col_i16 b);

#include "control.h"
#include "nations.h"
#include "goods.h"
#include "difficul.h"
#include "jobs.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */







extern void seed_random(col_i16 n);
extern void * memset_rtl(void *p, col_i16 c, col_u16 n);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
/* same object: 4-byte `push cs; call ` (rule 47) */




void europe_init_nations(void)
{
    col_i16 range;
    col_i16 roll;
    col_i16 price;
    col_i16 k;
    register col_i16 g;
    register col_i16 n;

    seed_random(g_random_seed);
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        set_current_nation(n);
        g_cur_nation_rec->flags = 0;
        g_cur_nation_rec->gold = 0L;
        if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN) {
            if (g_difficulty == DIFFICULTY_DISCOVERER)
                g_cur_nation_rec->gold = 1000L;
            else if (g_difficulty == DIFFICULTY_EXPLORER)
                g_cur_nation_rec->gold = 300L;
        }
        g_cur_nation_rec->bells = 0;
        g_cur_nation_rec->fathers = 0;
        g_cur_nation_rec->next_father = -1;
        g_cur_nation_rec->bells_total = 0;
        g_cur_nation_rec->f10 = 0;
        memset_rtl(((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x7)), 0, 4);
        g_cur_nation_rec->crosses = 0;
        g_cur_nation_rec->f49 = 0;
        g_cur_nation_rec->horses_stock = 0;
        g_cur_nation_rec->muskets_stock = 0;
        g_cur_nation_rec->seed = rand_range(1, 0x20);
        g_cur_nation_rec->seed_add = rand_range(0, 0x1f);
        g_cur_nation_rec->founded = 0;
        g_cur_nation_rec->f18 = 0;
        memset_rtl(((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x1b)), 0, 2);
        g_cur_nation_rec->boycott = 0;
        g_cur_nation_rec->support = 0;
        g_cur_nation_rec->shown = 0;
        g_cur_nation_rec->artillery = 0;
        g_cur_nation_rec->tax = 0;
        ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[0] = g_difficulty < DIFFICULTY_VICEROY ? JOB_INDENTURED_SERVANT : JOB_CRIMINAL;
        ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[1] = pick_recruit(g_difficulty < DIFFICULTY_GOVERNOR ? 1 : 0);
        ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[2] = pick_recruit(1);
        if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN) {
            if (g_difficulty == DIFFICULTY_DISCOVERER) {
                ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[0] = JOB_CARPENTER;
                ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[1] = JOB_FARMER;
                ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[2] = JOB_SCOUT;
            } else if (g_difficulty == DIFFICULTY_EXPLORER) {
                ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[1] = JOB_FARMER;
                ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[2] = JOB_SCOUT;
            }
        }
        if (g_cur_nation == NATION_SPAIN)
            ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x2))[0] = JOB_MISSIONARY;
        g_cur_nation_rec->f16 = 0;
        g_cur_nation_rec->f22 = 0L;
        g_cur_nation_rec->f26 = 0L;
        g_cur_nation_rec->demands = 0;
        g_cur_nation_rec->f6 = 0;
        for (g = 0; g < GOODS_COUNT; g++) {
            ((col_i16 *)((unsigned char *)(g_cur_nation_rec) + 0x5c))[g] = 0;
            ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0x7c))[g] = 0L;
            ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0xbc))[g] = 0L;
            ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0xfc))[g] = 0L;
        }
        for (k = 0; k < 4; k++)
            ((unsigned char *)((unsigned char *)(g_cur_nation_rec) + 0x40))[k] = 0;
    }
    for (g = 0; g < GOODS_COUNT; g++) {
        range = g_goods[g].start_hi - g_goods[g].start_lo + 1;
        roll = rand_range(0, range - 1);
        price = g_goods[g].start_lo + roll;
        for (n = 0; n < NATION_EUROPEAN_COUNT; n++)
            ((char *)((unsigned char *)&(g_nations[n]) + 0x4c))[g] = price;
    }
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        set_current_nation(n);
        update_market_price(1, -1);
    }
}
