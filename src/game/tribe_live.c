/* tribe_live: candidate for 1030:c915 (1229 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1229, not the census 1220: a dead second epilogue and two bytes of padding before 1030:cde2.
 *
 * WHAT A NATIVE SETTLEMENT TEACHES, AND A COLONIST'S VISIT TO LEARN IT. Returns the profession the
 * settlement teaches. With `just_skill` it only works that out (`tribe_camp` names it this way).
 *
 * THE SKILL is fixed per settlement: the draw is seeded from the settlement's position and the
 * game seed at DGROUP:0x4bc2, between the two random-stream brackets (0x4b88, 0x4b86). It is a
 * weighted pick from the tribe's 16-word goods table (data segment 25, filled by `indian_supply_and_demand`).
 * Jobs 0..15 are exactly the experts who produce goods 0..15, so the pick is both a goods and a
 * profession. The weights are adjusted before the draw:
 *   - fishing is never drawn directly;
 *   - below level 1 no fur trader or ore miner, and half the farmers;
 *   - below level 2 no weaver, tobacconist or silver miner, and a quarter fewer farmers;
 *   - below level 3 no distiller, and at level 3 half again as many silver miners.
 * A fur trapper on a square whose x + y is a multiple of 3 becomes a seasoned scout. A farmer
 * becomes a fisherman when more of the 20 surrounding squares are water than a roll of 1..20.
 * Occasional ambient native music plays for a human (tribes 0 and 1 have their own).
 *
 * THE VISIT answers with "LEARN" + an outcome, shown to a human nation:
 *   - MAD when the tribe's mood toward the nation is above 1; the alarm rises by 3, and at one
 *     pair-flag state (0x60 of the nation pair is 0x20) nothing is shown;
 *   - CRIMINAL for a petty criminal; a convert gets TEACHCONVERT instead;
 *   - MASTER for anyone already a specialist;
 *   - ALREADY when the settlement has taught (flag 2) and flag 4 is clear;
 *   - SLOW with a mood above 0 on a roll under `100 + 200 * difficulty` in 1000;
 *   - LATER when the human declines LEARNSTAY;
 *   - DONE: the colonist's specialty becomes the skill and the settlement is marked as having taught.
 * Only a free colonist (JOB_COLONIST_ALT) or an indentured servant can learn.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from learn_from_natives: the Mac build's tribe_live(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller tribe_camp; caller tribe_village; callee get_treaty; callee indian_supply_and_demand; callee pissed), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "control.h"
#include "tribes.h"
#include "settle.h"
#include "nations.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_mood(int mood);
extern void random_activate(int seed);
extern void random_reseed(long seed);
extern int terrain_is_ocean(int x, int y);
extern void popup_say(int slot, int word);
extern int pissed(int tribe, int i);
extern int piss_factor(int tension);

extern void piss_off(int tribe, int nation, int delta, int unused);
extern int popi(char *msg, int tribe);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void indian_supply_and_demand(void);
extern int get_name(int nation);
extern int get_treaty(int nation_a, int nation_b);

int tribe_live(int u, int nation, int tribe_nation, int just_skill)
{
    int sum;
    int roll;
    int skill;
    int spec;
    int human;
    int water;
    int nx;
    int ny;
    long seed;
    char msg[10];
    char key[20];
    register int dir;
    register int i;

    human = nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN ? 1 : 0;
    if (human && rand_range(0, 3) == 0) {
        set_mood(5);
        if (g_cur_tribe == 0)
            set_mood(7);
        if (g_cur_tribe == 1)
            set_mood(6);
    }
    indian_supply_and_demand();
    random_activate(g_event_seed);
    seed = g_cur_settlement_rec->x + g_seed_base + ((long)g_cur_settlement_rec->y << 8);
    random_reseed(seed);
    g_native_sell[JOB_FISHERMAN] = 0;
    if (g_cur_tribe_rec->level < LEVEL_AGRARIAN) {
        g_native_sell[JOB_FUR_TRADER] = 0;
        g_native_sell[JOB_ORE_MINER] = 0;
        g_native_sell[JOB_FARMER] = g_native_sell[JOB_FARMER] >> 1;
    }
    if (g_cur_tribe_rec->level < LEVEL_ADVANCED) {
        g_native_sell[JOB_WEAVER] = 0;
        g_native_sell[JOB_TOBACCONIST] = 0;
        g_native_sell[JOB_SILVER_MINER] = 0;
        g_native_sell[JOB_FARMER] -= g_native_sell[JOB_FARMER] >> 2;
    }
    if (g_cur_tribe_rec->level < LEVEL_CIVILIZED)
        g_native_sell[JOB_DISTILLER] = 0;
    if (g_cur_tribe_rec->level == LEVEL_CIVILIZED)
        g_native_sell[JOB_SILVER_MINER] += g_native_sell[JOB_SILVER_MINER] >> 1;
    sum = 0;
    for (i = 0; i < 16; i++)
        sum += g_native_sell[i];
    roll = rand_range(1, sum);
    skill = -1;
    do {
        skill++;
        roll -= g_native_sell[skill];
    } while (roll > 0);
    if (skill == JOB_FUR_TRAPPER && (g_cur_settlement_rec->x + g_cur_settlement_rec->y) % 3 == 0)
        skill = JOB_SCOUT;
    if (skill == JOB_FARMER) {
        water = 0;
        for (dir = 0; dir < 20; dir++) {
            nx = g_cur_settlement_rec->x + g_ring_dx[dir];
            ny = g_cur_settlement_rec->y + g_ring_dy[dir];
            if (terrain_is_ocean(nx, ny))
                water++;
        }
        if (rand_range(1, 20) < water)
            skill = JOB_FISHERMAN;
    }
    random_activate(g_random_seed);
    if (just_skill != 0)
        goto out;
    spec = g_units[u].spec;
    popup_say(0, get_name(tribe_nation));
    popup_say(1, g_spec_recs[skill].name);
    if (piss_factor(pissed(g_cur_tribe, nation)) > 1) {
        STRCPY(msg, "MAD");
        piss_off(g_cur_tribe, nation, 3, 0);
        if ((get_treaty(nation, tribe_nation) & 0x60) == 0x20)
            goto out;
        goto show;
    }
    if (spec == JOB_CRIMINAL) {
        STRCPY(msg, "CRIMINAL");
        goto show;
    }
    if (spec == JOB_CONVERT) {
        show_text_dialog_1030_ec0a("TEACHCONVERT");
        goto out;
    }
    if (spec != JOB_COLONIST_ALT && spec != JOB_INDENTURED_SERVANT) {
        popup_say(1, g_spec_recs[spec].name);
        STRCPY(msg, "MASTER");
        goto show;
    }
    if ((g_cur_settlement_rec->flags & SETTLEMENT_TAUGHT) && !(g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)) {
        STRCPY(msg, "ALREADY");
        goto show;
    }
    random_activate(g_random_seed);
    if (piss_factor(pissed(g_cur_tribe, nation)) > 0
        && rand_range(1, 1000) < g_game.difficulty * 200 + 100) {
        STRCPY(msg, "SLOW");
        goto show;
    }
    if (human && popi("LEARNSTAY", g_cur_tribe) != 1) {
        STRCPY(msg, "LATER");
        goto show;
    }
    STRCPY(msg, "DONE");
    g_units[u].spec = skill;
    g_cur_settlement_rec->flags |= SETTLEMENT_TAUGHT;
show:
    STRCPY(key, "LEARN");
    STRCAT(key, msg);
    if (human)
        popi(key, g_cur_tribe);
out:
    return skill;
}
