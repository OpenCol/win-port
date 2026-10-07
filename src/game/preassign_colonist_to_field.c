/* preassign_colonist_to_field: candidate for 1018:7327 (1819 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 * size-source: 1819, not the census 1810. The trailing 9 bytes are the dead second epilogue.
 *
 * WHERE A COLONIST SHOULD WORK. For colonist `c` of the current colony it scores every free square
 * of the colony's ring against every outdoor job (0..8, JOB_FARMER through JOB_FISHERMAN) and puts
 * the colonist on the best one. `try_to_set_job`, `1018:04e0` and `1018:7a42` call it.
 * It answers 0 when the colonist was given work and 1 when it found none.
 *
 * `want` steers it. Below zero it is the planner: every job is tried and the colonist is moved.
 * At or above zero only that one job is tried, and most of the weighting below is skipped. At -2
 * or below the scores are computed and the colonist is put back where it was -- the caller only
 * wants the two figures left in DGROUP:0x4bcc (the amount) and 0x4bce (the score).
 *
 * THE SETUP parks the colonist on JOB_TEACHER and recomputes the colony's production, so every
 * score below is measured against a colony this colonist contributes nothing to. `food_short` is
 * whether food production is under the target at SEG21:0x164; it is cancelled while DGROUP:0xc28
 * is set, and for a computer colony also when the warehouse is already fuller than its capacity or
 * than sixteen times the food overflow. With no food shortage, a human European colony that has no
 * carpenter, three or more colonists and a non-negative byte at +0x94 makes this colonist a
 * JOB_CARPENTER and stops there.
 *
 * THE SCORE for a square and a job: the production (`compute_yield`), cut to the room
 * left in the warehouse for that goods, times eight, plus `7 - (|col-2| + |row-2|)` so the inner
 * ring wins ties, and doubled when it is the job the colonist already had.
 *
 *   - WITH A FOOD SHORTAGE food and fish are boosted and other jobs keep their plain score. Fish first gains 8, then both are multiplied by 32, then charged the
 *     terrain's byte at SEG20:0x78f0+0xe -- plus 0x18 on a forest square while the colony has ten
 *     lumber or less and cuts none -- and a square a tribe claims is halved.
 *   - OTHERWISE the goods is priced from the nation's value table at SEG30:0x10, except food and fish,
 *     which are 4 in a colony whose population is under twice the ring size while DGROUP:0xc28 is set
 *     and 0 otherwise (raised to 1 for a human colony). A computer nation
 *     with eight colonists after turn 80 adds to ore. That price plus one is the weight: one more
 *     if the goods is already overflowing (and the score doubles past the whole band), one less for
 *     lumber the colony has two or more of, plus the levels of the building the job feeds, less what
 *     the tribe claiming the square would cost -- `4 - pissed`, doubled where the tribe is the
 *     stronger in the region, half again where it is the stronger overall, and offset by the armed
 *     units standing in the colony. The final score is `(weight + price) * score`, and ten is taken
 *     off for working a forest square at anything but lumber while lumber is short.
 *
 * SHIPPED QUIRKS. That last test reads `terrain`, which is only ever written inside the
 * food-shortage arm -- so outside a food shortage it is whatever the previous square left there,
 * and on the first square of the call it is uninitialised stack. The price is counted twice, once
 * in `weight` and once in `weight + price`. And `kind = best_kind` on the way out is a dead store.
 *
 * WHAT THE BYTES SETTLED: `want >= 0` skips the whole weighting through a `goto` (rule 12's
 * JCC-over-JMP), while the food-shortage split beside it is a real `if/else` (a 386 long conditional
 * jump). `value` is assigned in two arms, not a ternary -- both stores are to the slot. The
 * production table at SEG21:0x122 is signed and the one at SEG25:0x8 unsigned; both were found by
 * a single flipped jump. `score` and `job` are the two `register` locals, SI and DI in declaration
 * order (rule 53), the ring count comes through `g_ring_tile_count[level - 1]` as everywhere else,
 * and `max_ptr_1018_b55a` is the one NOP-padded 5-byte call, so it alone is extern (rule 47).
 * *
 * `g_best_job_score` (DGROUP:0x4bce) is this function's second result, returned through a
 * global beside `g_best_job_amount`. `colony_plan` is what reads it: it shifts the score
 * left two and adds one for an unusual specialty and two for a match against `g_wanted_specialty`,
 * which makes those the tie-breakers under the score rather than beside it.
 *
 * RENAMED 2026-09-27 from best_colonist_job: Mac preassign_colonist_to_field(short, short), 1934 bytes, against 1819. Callers: colony_plan, colony_select_profession and preassign_colonists_to_fields (7a42) on both. The 15 Mac callees are the Windows callee set, with min/max as Windows inline helpers. Arity: 2 = (colonist, want). mac_bind: strong on all three methods (votes 17/2, align 10/0). Layout: the Mac CODE 7 run colony_indian_table, pick_up_invalid_colonists, preassign_colonist_to_field, preassign_colonists_to_fields, num_cargo_vessels(void), nth_cargo_vessel is the Windows 1018 run colony_indian_table, 72e4, 7327, 7a42, 7af0, nth_cargo_vessel, one for one between bound neighbours.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "building.h"
#include "jobs.h"
#include "goods.h"
#include "terrain.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_consumption[];                     /* SEG21:0x164 */
extern int g_deficit[];                   /* SEG21:0x18c */
extern int g_shortfall[];                 /* SEG21:0x1b4 */

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int land_region_at(int x, int y);

extern int pissed(int tribe, int i);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int get_warehouse(void);
extern "C" int get_job(int colonist);
extern "C" int set_job(int colonist, int job);
extern "C" void compute_colony_yield(void);
extern "C" int job_count(int job);
extern "C" int colony_map_status(void);
extern "C" void set_field(int col, int row, int colonist);
extern "C" int compute_yield(int col, int row, int *kind_out, int flag);

extern "C" int job_is_useful(int job);
extern "C" int building_level(int bit);

int preassign_colonist_to_field(int colonist, int want)
{
    int result;
    int col;
    int row;
    int orig_kind;
    int best_col;
    int best_row;
    int best_score;
    int best_kind;
    int best_amount;
    int kind;
    int amount;
    int weight;
    int food_short;
    int human;
    int i;
    int ring_n;
    int value;
    int terrain;
    int tcost;
    int saved;
    int penalty;
    int nation;
    int claim;
    int room;
    int capacity;
    int armed;
    int region;
    int t1;
    int t2;
    register int score;
    register int job;

    result = 1;
    armed = 0;
    region = 0;
    nation = g_cur_colony_rec->nation;
    human = nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN;
    capacity = get_warehouse();
    if (human == 0) {
        saved = colonist;
        colonist = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
        while (colonist >= 0) {
            if (g_unit_types[g_units[colonist].type].defence > 1)
                armed++;
            colonist = next_unit(colonist);
        }
        colonist = saved;
        region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    }
    best_score = 0;
    best_amount = 0;
    best_kind = -1;
    orig_kind = get_job(colonist);
    set_job(colonist, JOB_TEACHER);
    compute_colony_yield();
    food_short = g_prod[GOODS_FOOD] < g_consumption[GOODS_FOOD];
    if (g_ai_managing_colony != 0)
        food_short = 0;
    if (human == 0) {
        if (get_warehouse() < g_cur_colony_rec->stock[GOODS_FOOD])
            food_short = 0;
        if (g_deficit[GOODS_FOOD] << 4 < g_cur_colony_rec->stock[GOODS_FOOD])
            food_short = 0;
    }
    if (food_short == 0 && want < 0 && human != 0
        && job_count(JOB_CARPENTER) == 0
        && (int)g_cur_colony_rec->pop >= 3
        && (int)g_cur_colony_rec->flags[0x10] >= 0) {
        set_job(colonist, JOB_CARPENTER);
        result = 0;
        goto done;
    }
    g_trial_placement = 1;
    ring_n = g_ring_tile_count[colony_map_status() - 1];
    for (i = 0; i < ring_n; i++) {
        col = g_ring_dx[i] + 2;
        row = g_ring_dy[i] + 2;
        if (g_grid_marks[col][row] != 0)
            continue;
        if (human != 0 && (int)g_tile_tribe[col][row] >= 0)
            continue;
        if ((int)g_cur_colony_rec->slots[i] >= 0)
            continue;
        set_field(col, row, colonist);
        for (job = 0; job < 9; job++) {
            if (want >= 0 && job != want)
                continue;
            set_job(colonist, job);
            amount = compute_yield(col, row, &kind, 0);
            score = amount;
            if (want < 0) {
                room = capacity - g_cur_colony_rec->stock[kind];
                room = max_int_1018_b34f(1, room);
                score = min_int_1018_b329(score, room);
            }
            score = score << 3;
            if (score != 0)
                score += 7 - ((col - 2 > 0 ? col - 2 : ~(col - 2) + 1)
                              + (row - 2 > 0 ? row - 2 : ~(row - 2) + 1));
            if (want < 0 && job == orig_kind && human != 0)
                score = score * 2;
            if (want >= 0)
                goto compare;
            {
                if (food_short != 0) {
                    if (job == JOB_FARMER || job == JOB_FISHERMAN) {
                        if (job == JOB_FISHERMAN && score != 0)
                            score += 8;
                        score = score << 5;
                        if (score != 0) {
                            terrain = terrain_at(g_cur_colony_rec->x + col - 2,
                                                 g_cur_colony_rec->y + row - 2);
                            tcost = (unsigned char)g_terrain_rows[terrain].best_goods_value;
                            if (terrain >= TERRAIN_BOREAL_FOREST && terrain < TERRAIN_ARCTIC
                                && g_cur_colony_rec->stock[GOODS_LUMBER] <= 0xa
                                && g_prod[GOODS_LUMBER] == 0)
                                tcost += 0x18;
                            score = max_int_1018_b34f(1, score - tcost);
                        }
                    }
                    if ((int)g_tile_tribe[col][row] >= 0)
                        score = score >> 1;
                } else {
                    if (job == JOB_FISHERMAN || job == JOB_FARMER) {
                        if (g_cur_colony_rec->pop < (signed char)(ring_n * 2) && g_ai_managing_colony != 0)
                            value = 4;
                        else
                            value = 0;
                        if (human != 0 || want >= 0)
                            value = max_int_1018_b34f(1, value);
                    } else {
                        value = g_goods_value[nation][kind];
                        if (human == 0 && kind == GOODS_ORE && (int)g_cur_colony_rec->pop >= 8
                            && g_game.turn_counter >= 0x50) {
                            value += 2;
                            if (g_rank[nation] >= g_rank[g_game.player_nation]) {
                                value += building_level(BUILDING_BLACKSMITHS_SHOP);
                                value += building_level(BUILDING_ARMORY) * 2;
                            }
                        }
                    }
                    weight = value + 1;
                    if (want < 0) {
                        if (g_deficit[kind] != 0 || g_shortfall[kind] != 0) {
                            weight++;
                            if (g_shortfall[kind] != 0)
                                score = score * 2;
                        } else if (job == JOB_LUMBERJACK
                                   && (unsigned)(g_cur_colony_rec->stock[GOODS_LUMBER] + g_prod[GOODS_LUMBER]) >= 2)
                            weight--;
                        weight += job_is_useful((int)g_paired_job[job]);
                        claim = g_tile_tribe[col][row];
                        if (claim >= 0) {
                            penalty = 4 - pissed(claim, nation);
                            if (g_nation_region[nation][region] < g_tribe_region[claim][region])
                                penalty = penalty * 2;
                            if (g_nation_land_strength[nation] < g_tribe_strength[claim])
                                penalty = (penalty * 2 + penalty) >> 1;
                            penalty = *max_ptr_1018_b55a((t2 = 0, &t2),
                                                         (t1 = penalty - armed, &t1));
                            weight -= penalty;
                        }
                    }
                    weight = max_int_1018_b34f(0, weight);
                    score = (weight + value) * score;
                    if (want < 0 && kind != GOODS_LUMBER && terrain >= TERRAIN_BOREAL_FOREST && terrain < TERRAIN_ARCTIC
                        && g_cur_colony_rec->stock[GOODS_LUMBER] <= 0xa && g_prod[GOODS_LUMBER] == 0)
                        score -= 0xa;
                }
            }
compare:
            if (score > best_score) {
                best_score = score;
                best_kind = kind;
                best_amount = amount;
                best_col = col;
                best_row = row;
            }
        }
        set_field(col, row, -1);
    }
    g_trial_placement = 0;
    if (best_kind >= 0)
        result = 0;
    if (best_kind >= 0 && want > -2) {
        set_field(best_col, best_row, colonist);
        set_job(colonist, best_kind);
    } else {
        set_job(colonist, orig_kind);
        best_amount = 0;
    }
done:
    g_best_job_amount = best_amount;
    g_best_job_score = best_score;
    kind = best_kind;
    return result;
}
