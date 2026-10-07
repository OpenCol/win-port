/* compute_colonist_yield: candidate for 1018:6a20 (745 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -a
 * name-source: sibling-build
 * size-source: 745, the code and its 9-entry jump table. As with `CheatMenuColonist`, one pad byte the
 *   object does not emit sits before the table, so the 746-byte gap is one more than compares.
 *
 * What colonist `c` of the current colony makes at a building job, and optionally which good.
 *
 * The base is 3 for an expert at the job (specialty equal to job) and 1 otherwise, and that is the
 * answer for any job outside JOB_DISTILLER..JOB_STATESMAN. For those the unit rate is 1 for a
 * JOB_CRIMINAL or JOB_CONVERT specialty, 2 for JOB_INDENTURED_SERVANT and 3 for anyone else, and a
 * penalty is applied: minus the colony's Tory share --
 * (100 - SoL%) of its population, rounded -- divided by 10 less the difficulty for a nation below 4
 * whose byte at SEG20:0x7841 is clear, and 0 for anyone else; plus 1 for each of bits 4 and 2 of
 * the colony's byte +0x1c. Then:
 *
 *   13       6 for an expert, else the rate, plus the penalty; doubled when colony flag 0x24 is set.
 *            The good is 0x10.
 *   16       the same with colony flag 0x26, and half again when nation flag 0x15 is set. Good 0x11.
 *   17       the rate plus the penalty, doubled for an expert. Good 0x12.
 *   9-12,14,15  the rate plus the penalty, plus the rate again with more than one of the building,
 *            plus half with more than two, doubled for an expert. The good is the job number.
 *
 * The result is halved when the building's level byte (SEG20:0xd42b) is positive and the nation's
 * byte at +0 has bit 0x10 set, and never goes below 0. The good (or -1) goes to `*good` when given.
 * Cases 15 and 9-12,14 are separate identical bodies in the shipped code.
 *
 * Read against the game's rules the numbers fit the manufacturing buildings -- a reading, not
 * something the bytes show: jobs 9-12, 14 and 15 make their own good, 13 makes good 0x10 at 6 for an
 * expert (hammers), 16 good 0x11 with a nation-flag +50% (crosses), 17 good 0x12 (bells); a second
 * building level adds a unit and a third half again; and the three low rates are the indentured
 * servant, petty criminal and native convert.
 *
 * RENAMED 2026-09-26 from building_output: the Mac build's compute_colonist_yield(short, short*), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_plan; caller colony_select_profession; callee colony_rebel; callee get_job; callee get_specialty), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

#include "colflag.h"
#include "goods.h"
#include "jobs.h"
#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "building.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
                                       /* 12 bytes at SEG20:0xd426; 0xd42b is +5, .level */

extern GameState g_game;   /* SEG20:0x7782 */
extern Nation g_nations[];             /* SEG20:0xc2f4 */

extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_job(int colonist);
extern int get_specialty(int u);
extern int colony_rebel(void);
extern int building_of_job(int job);
extern int best_building(int bit);
extern int has_building(int bit);
extern int building_level(int bit);

int compute_colonist_yield(int colonist, int *good)
{
    int job;
    int spec;
    int expert;
    int out;
    int slot;
    int top;
    int n;
    int tories;
    int sol;
    int div;
    int pen;
    int halve;
    register int base;
    register int rate;

    halve = 0;
    job = get_job(colonist);
    spec = get_specialty(colonist);
    expert = spec == job ? 1 : 0;
    sol = colony_rebel();
    tories = (g_cur_colony_rec->pop * (100 - sol) + 50) / 100;
    div = g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_colony_rec->nation].control == CONTROL_HUMAN
        ? 10 - g_game.difficulty : 10;
    if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
        tories = 0;
    pen = -(tories / div);
    if (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY)
        pen++;
    if (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS)
        pen++;
    base = expert ? 3 : 1;
    out = -1;
    slot = building_of_job(job);
    top = best_building(slot);
    if ((int)g_buildings[top].upkeep > 0 && (g_nations[g_cur_colony_rec->nation].flags & 0x10))
        halve = 1;
    switch (spec) {
    case JOB_CRIMINAL:
    case JOB_CONVERT:
        rate = 1;
        break;
    case JOB_INDENTURED_SERVANT:
        rate = 2;
        break;
    default:
        rate = 3;
        break;
    }
    switch (job) {
    case JOB_CARPENTER:
        out = PROD_HAMMERS;
        base = expert ? 6 : rate;
        base += pen;
        if (has_building(BUILDING_LUMBER_MILL))
            base = base * 2;
        break;
    case JOB_PREACHER:
        base = expert ? 6 : rate;
        out = PROD_CROSSES;
        base += pen;
        if (has_building(BUILDING_CATHEDRAL))
            base = base * 2;
        if (has_father(g_cur_colony_rec->nation, FATHER_WILLIAM_PENN))
            base += base >> 1;
        break;
    case JOB_DISTILLER:
    case JOB_TOBACCONIST:
    case JOB_WEAVER:
    case JOB_FUR_TRADER:
    case JOB_BLACKSMITH:
        out = job;
        base = rate;
        base += pen;
        n = building_level(slot);
        if (n > 1)
            base += rate;
        if (n > 2)
            base += base >> 1;
        if (expert)
            base = base * 2;
        break;
    case JOB_GUNSMITH:
        out = job;
        base = rate;
        base += pen;
        n = building_level(slot);
        if (n > 1)
            base += rate;
        if (n > 2)
            base += base >> 1;
        if (expert)
            base = base * 2;
        break;
    case JOB_STATESMAN:
        base = rate;
        base += pen;
        out = PROD_BELLS;
        if (expert)
            base = base * 2;
        break;
    }
    if (halve)
        base = base >> 1;
    if (good != 0)
        *good = out;
    base = max_int_1018_b34f(0, base);
    return base;
}
