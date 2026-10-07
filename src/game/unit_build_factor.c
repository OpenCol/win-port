/* unit_build_factor: candidate for 1028:08d3 (322 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The third and negative term of `unit_build_another`: it can only ever return a
 * value AT OR BELOW ZERO, because the last thing it does is `min(0, score)`. The caller then
 * clamps the sum of all three terms at zero from the other side.
 *
 * What it charges for: a nation that already has colonies pays the distance to its nearest
 * one, divided by five, less one -- or a flat +2 when `find_close_colony` finds none within
 * range. Then the unit's own type moves it (+2 for 2, -2 for 1, -3 for 4) and type 0, the
 * plain colonist, pays -2 outright, another -2 if `any_spec` says it has a real
 * speciality (not one of the common ones), and a further -20 for speciality 0x1b. Finally, when
 * `build_another_colony` agrees, a sixteenth of the gap between the TURN COUNTER
 * (SEG20:0x7790) and the nation record's
 * word at +0x46.
 *
 * -20 dwarfs everything else, so speciality 0x1b alone is enough to drive the whole term to
 * its floor.
 *
 * TWO SPELLINGS THE BYTES FIXED, both of them about writing things out. `(int)spec == 0x1b`
 * carries a written-out cast -- `MOV AL / CBW / CMP AX,0x1b` where the plain comparison folds
 * to one `CMP BYTE` (rule 51) -- and the `min` result is ASSIGNED BACK to the score and then
 * returned, which is the `MOV SI,AX / MOV AX,SI` pair before the epilogue; `return min(...)`
 * has neither.
 *
 * `u` is a `register` PARAMETER and `score` a `register` local, so `score` takes SI and `u`
 * takes DI: rule 53, a register parameter is allocated after every register local.
 *
 * RENAMED 2026-09-26 from unit_score_penalty: the Mac build's unit_build_factor(short, short, short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller plan_move; callee any_spec; callee find_close_colony), the runner-up gets 1; the arity agrees (3), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "records.h"

extern Nation g_nations[];                    /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */
extern int find_close_colony(int x, int y, int nation, int land);
extern int any_spec(int job);

/* same object: 4-byte `push cs; call ` */
extern int build_another_colony(int nation);

int unit_build_factor(int nation, register int u, int land)
{
    register int score = 0;

    if (g_nation_colony_count[nation] != 0) {
        if (find_close_colony(g_units[u].x, g_units[u].y, nation, land) >= 0)
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
        if (any_spec(g_units[u].spec) != 0)
            score -= 2;
        if ((int)g_units[u].spec == JOB_CONVERT)
            score -= 20;
    }
    if (build_another_colony(nation) != 0)
        score += (g_game.turn_counter - g_nations[nation].founded) >> 4;
    score = min_int_1028_e376(0, score);
    return score;
}
