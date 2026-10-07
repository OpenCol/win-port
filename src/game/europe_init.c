/* europe_init: candidate for 1048:81c1 (918 bytes).  Verdict: RELOCATABLE.
 * symbol: europe_init
 * name-source: sibling-build
 *
 * New-game setup of the four European nations' records (0x13c bytes at SEG20:0xc2f4), made current one
 * at a time: every counter, long and per-goods array cleared, two random bytes (+0x44 in 1..32, +0x45 in
 * 0..31), and then the three things a new player sees first.
 *
 * STARTING GOLD (for a nation whose SEG20:0x7841 control byte is 0, the human player): 1000 at Discoverer
 * (difficulty 0), 300 at Explorer (1), nothing above that or for
 * a computer nation.
 *
 * THE THREE COLONISTS WAITING ON THE DOCKS (+2..+4, the pool `europe_new_turn` draws emigrants from):
 * normally an INDENTURED SERVANTS (JOB25, below Viceroy) or PETTY CRIMINALS (JOB26) and two picks
 * from `fill_docks`. A human player on Discoverer instead starts with EXPERT CARPENTERS,
 * EXPERT FARMERS and SEASONED SCOUTS (JOB13, JOB0, JOB22), on Explorer the farmers and scouts. SPAIN
 * always replaces the first with JESUIT MISSIONARIES (JOB24) -- even for a human player at the easiest
 * level, since that test comes last.
 *
 * THE MARKET: each goods' price is drawn once, uniformly from its low..high range in the 10-byte table at
 * SEG20:0xd79c, and the SAME price is written into all four nations' price bytes (+0x4c); every nation's
 * market is then brought up to date with `europe_adjust_prices(1, -1)`. The random generator is seeded
 * from DGROUP:0x4b86 first, so a given seed reproduces the prices.
 *
 * RENAMED 2026-09-26 from europe_init_nations: the Mac build's europe_init(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee europe_adjust_prices; callee fill_docks; callee set_europe), the runner-up gets 2, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "goods.h"
#include "difficul.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];               /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern void random_activate(int seed);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_europe(int n);
extern unsigned char fill_docks(int from_pool);
extern void europe_adjust_prices(int reset, int only);

void europe_init(void)
{
    int range;
    int roll;
    int price;
    int k;
    register int goods;
    register int n;

    random_activate(g_random_seed);
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        set_europe(n);
        g_cur_nation_rec->flags = 0;
        g_cur_nation_rec->gold = 0L;
        if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
            if (g_game.difficulty == DIFFICULTY_DISCOVERER)
                g_cur_nation_rec->gold = 1000L;
            else if (g_game.difficulty == DIFFICULTY_EXPLORER)
                g_cur_nation_rec->gold = 300L;
        }
        g_cur_nation_rec->bells = 0;
        g_cur_nation_rec->fathers = 0;
        g_cur_nation_rec->next_father = -1;
        g_cur_nation_rec->bells_total = 0;
        g_cur_nation_rec->last_tax_delta = 0;
        memset_rtl(g_cur_nation_rec->father_bits, 0, 4);
        g_cur_nation_rec->crosses = 0;
        g_cur_nation_rec->muskets = 0;
        g_cur_nation_rec->horses = 0;
        g_cur_nation_rec->artillery_credit = 0;
        g_cur_nation_rec->seed = rand_range(1, 0x20);
        g_cur_nation_rec->seed_add = rand_range(0, 0x1f);
        g_cur_nation_rec->founded = 0;
        g_cur_nation_rec->villages_burned = 0;
        memset_rtl(g_cur_nation_rec->f1b, 0, 2);
        g_cur_nation_rec->boycott = 0;
        g_cur_nation_rec->support = 0;
        g_cur_nation_rec->shown = 0;
        g_cur_nation_rec->artillery = 0;
        g_cur_nation_rec->tax = 0;
        g_cur_nation_rec->recruit[0] = g_game.difficulty < DIFFICULTY_VICEROY ? JOB_INDENTURED_SERVANT : JOB_CRIMINAL;
        g_cur_nation_rec->recruit[1] = fill_docks(g_game.difficulty < DIFFICULTY_GOVERNOR ? 1 : 0);
        g_cur_nation_rec->recruit[2] = fill_docks(1);
        if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
            if (g_game.difficulty == DIFFICULTY_DISCOVERER) {
                g_cur_nation_rec->recruit[0] = JOB_CARPENTER;
                g_cur_nation_rec->recruit[1] = JOB_FARMER;
                g_cur_nation_rec->recruit[2] = JOB_SCOUT;
            } else if (g_game.difficulty == DIFFICULTY_EXPLORER) {
                g_cur_nation_rec->recruit[1] = JOB_FARMER;
                g_cur_nation_rec->recruit[2] = JOB_SCOUT;
            }
        }
        if (g_cur_nation == NATION_SPAIN)
            g_cur_nation_rec->recruit[0] = JOB_MISSIONARY;
        g_cur_nation_rec->f16 = 0;
        g_cur_nation_rec->royal_gold = 0L;
        g_cur_nation_rec->net_take = 0L;
        g_cur_nation_rec->grants_taken = 0;
        g_cur_nation_rec->recruit_count = 0;
        for (goods = 0; goods < GOODS_COUNT; goods++) {
            g_cur_nation_rec->drift[goods] = 0;
            g_cur_nation_rec->trade_gold[goods] = 0L;
            g_cur_nation_rec->trade_tons[goods] = 0L;
            g_cur_nation_rec->sold[goods] = 0L;
        }
        for (k = 0; k < 4; k++)
            g_cur_nation_rec->timer[k] = 0;
    }
    for (goods = 0; goods < GOODS_COUNT; goods++) {
        range = g_goods[goods].start_hi - g_goods[goods].start_lo + 1;
        roll = rand_range(0, range - 1);
        price = g_goods[goods].start_lo + roll;
        for (n = 0; n < NATION_EUROPEAN_COUNT; n++)
            g_nations[n].price[goods] = price;
    }
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        set_europe(n);
        europe_adjust_prices(1, -1);
    }
}
