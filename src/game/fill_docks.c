/* fill_docks: candidate for 1048:6c51 (484 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 484, not the census 475. The trailing 9 bytes are the dead second epilogue.
 *
 * CHOOSES THE COLONIST WHO APPEARS ON THE EUROPE DOCKS to replace one who emigrated.
 * `europe_new_turn` calls it with `!(turn & 3)`, so three turns in four go through the
 * weighted draw and every fourth straight to the pool.
 *
 * THE DRAW. `d` is the difficulty plus one for a human nation, 2 for everyone else, and three
 * rolls against `(d + 2) >> 1` give, in order: type 0x1a (1 in 15 at the easiest), 0x19
 * (1 in 10), and 0x1c (1 in 8). Nation flag 0x14 turns the first two into 0x1c as well. That
 * flag is William Brewster -- `europe_new_turn` tests the same bit to let the player CHOOSE
 * the emigrant, which with this function's refusal to produce the two lowest classes is both
 * of his effects in the game.
 *
 * THE POOL. When none of the three docks slots holds an ordinary job (all three are specialists), the answer is 0x1c.
 * Otherwise a per-nation Galois LFSR at +0x44 (taps 0x14) is stepped and added to +0x45, masked
 * to 5 bits and redrawn until it is below 0x19; seven codes are folded onto others (0x12 and
 * 0x17 onto 0xd, 0x13 onto 0x16, 1..4 onto 8, 5, 0 and 6), and a result already on the docks
 * starts the pool again. After 100 draws the last one is accepted, duplicate or not.
 *
 * The retry is a `goto` back into the `else` branch -- the two-jump form -- and both early
 * exits are `goto done` (rule 12); the written-out `return` leaves a dead second epilogue
 * (rule 44). `i` and `d` are two `register` locals and take SI and DI in that order (rule 53).
 *
 * RENAMED 2026-09-26 from pick_recruit: the Mac build's fill_docks(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller europe_new_turn; caller europe_recruit; caller foreign_country; callee has_father), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int has_father(int nation, int bit);
extern int any_spec(int job);
extern int crc(int *p, int taps);

unsigned char fill_docks(int from_pool)
{
    int r;
    int found;
    int tries;
    register int i;
    register int d;

    tries = 0;
    if (from_pool != 0)
        goto pool;
    d = g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN ? g_game.difficulty : 1;
    d++;
    if (rand_range(1, 15) <= (d + 2) >> 1) {
        r = has_father(g_cur_nation, FATHER_WILLIAM_BREWSTER) ? JOB_COLONIST_ALT : JOB_CRIMINAL;
    } else if (rand_range(1, 10) <= (d + 2) >> 1) {
        r = has_father(g_cur_nation, FATHER_WILLIAM_BREWSTER) ? JOB_COLONIST_ALT : JOB_INDENTURED_SERVANT;
    } else if (rand_range(1, 8) <= (d + 2) >> 1) {
        r = JOB_COLONIST_ALT;
    } else {
pool:
        found = 0;
        for (i = 0; i < 3; i++)
            if (any_spec(g_cur_nation_rec->recruit[i]) == 0)
                found = 1;
        if (found == 0) {
            r = JOB_COLONIST_ALT;
        } else {
            do {
                r = g_cur_nation_rec->seed;
                g_cur_nation_rec->seed = crc(&r, 0x14);
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
                if (g_cur_nation_rec->recruit[i] == r)
                    goto pool;
        }
    }
done:
    return r;
}
