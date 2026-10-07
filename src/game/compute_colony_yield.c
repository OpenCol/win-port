/* compute_colony_yield: candidate for 1018:544c (1403 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE CURRENT COLONY'S PRODUCTION FOR ONE TURN, into the table at SEG21:0x122 (one word per goods,
 * with crosses at 17 and bells at 18). The colony window, the colony turn, the build menu and the
 * Sons of Liberty report all call it.
 *
 * THE COLONY SQUARE:
 *   - FOOD: arctic 0; desert or scrub forest 1; mountains, hills or any other forest 2; otherwise 3.
 *     Then +2 on Discoverer, +1 on Explorer, +1 plowed, +2 on a special resource of kinds 1, 2 or 9,
 *     and +1 each for colony flags 4 and 2. The result is kept in DGROUP:0x4ba4;
 *   - SECONDARY GOODS: the best of goods 1..7 except lumber, from the terrain yield plus the
 *     resource's bonus (a negative bonus doubles the yield instead). It is +1 on Discoverer,
 *     + river size, and +1 for each of the same two flags (0x4ba6 goods, 0x4ba7 amount).
 *
 * THE WORK AREA AND BUILDINGS: every worked square (`compute_yield`) and every
 * colonist's building output (`compute_colonist_yield`) are added in. Fishermen's catch is
 * also kept in 0x4ba8.
 *
 * CROSSES (17): 1, +1 for each of colony flags 0x25 and 0x26.
 * BELLS (18):
 *   - 1, +50% with Thomas Jefferson, + tax-rate percent with Thomas Paine;
 *   - doubled with colony flag 0x14, or +50% with flag 0x13.
 *
 * HORSES: with two or more, the colony can breed `2 x ceil(horses / step)`, with the step 25 given
 * colony flag 0x11 and 50 without. Breeding is limited by half the food surplus left after the
 * colonists eat (2 each) and by warehouse room. The food used is added to consumption, and the
 * unfed part is stored as g_shortfall[GOODS_HORSES] (SEG21:0x1c4). Consumption, hammers (g_prod[16]), goods 9..12 and 14, and muskets are then published.
 *
 * The signed goods byte is tested through `(int)` (rule 51).
 *
 * RENAMED 2026-09-26 from colony_production: the Mac build's compute_colony_yield(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_plan; callee feature_get; callee get_warehouse; callee has_building; callee has_father), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "difficul.h"
#include "colflag.h"
#include "building.h"
#include "terrain.h"
#include "goods.h"
#include "jobs.h"
#include "fathers.h"
#include "records.h"

extern int g_shortfall[];               /* SEG21:0x1b4; [GOODS_HORSES] is 0x1c4 */
extern Nation g_nations[];              /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char feature_get(int x, int y);
extern int resource_at(int x, int y);
extern unsigned char map_get(int x, int y);
extern int resource_bonus(int resource, int goods);
extern int compute_yield(int col, int row, int *kind_out, int flag);
extern int compute_colonist_yield(int colonist, int *kind_out);
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern char get_field(int col, int row);
extern int has_building(int bit);

extern int get_warehouse(void);
extern void simple_cash_flow(int slot, int use);
extern void factory_cash_flow(int slot, int index);

void compute_colony_yield(void)
{
    int i;
    int amt;
    int kind;
    int bamt;
    int bkind;
    int terrain;
    int amount;
    int horse_food;
    int breed;
    int used;
    int eat;
    int surplus;
    int deco;
    int river;
    int p0;
    int cap;
    int bonus;
    int step;
    int room;
    int zero;
    register int col;
    register int row;

    g_fish_food = 0;
    g_depletion_rolls = 0;
    terrain = terrain_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (terrain == TERRAIN_ARCTIC)
        g_center_food = 0;
    else if (terrain == TERRAIN_DESERT || terrain == TERRAIN_SCRUB_FOREST_2 || terrain == TERRAIN_SCRUB_FOREST)
        g_center_food = 1;
    else if (terrain == TERRAIN_MOUNTAINS || terrain == TERRAIN_HILLS
             || (terrain >= TERRAIN_BOREAL_FOREST && terrain < TERRAIN_BOREAL_FOREST_2)
             || (terrain >= TERRAIN_BOREAL_FOREST_2 && terrain < TERRAIN_ARCTIC))
        g_center_food = 2;
    else
        g_center_food = 3;
    if (g_game.difficulty == DIFFICULTY_DISCOVERER)
        g_center_food = g_center_food + 2;
    if (g_game.difficulty == DIFFICULTY_EXPLORER)
        g_center_food++;
    if (feature_get(g_cur_colony_rec->x, g_cur_colony_rec->y) & MAP1_PLOWED)
        g_center_food++;
    deco = resource_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    river = 0;
    p0 = map_get(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (p0 & MAP0_RIVER) {
        river++;
        if (p0 & MAP0_MAJOR_RIVER)
            river++;
    }
    if (deco == 1 || deco == 9 || deco == 2)
        g_center_food = g_center_food + 2;
    if (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY)
        g_center_food++;
    if (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS)
        g_center_food++;
    g_center_kind = -1;
    g_center_amount = 0;
    for (i = 1; i < 8; i++) {
        if (i == GOODS_LUMBER)
            continue;
        amount = g_terrain_rows[terrain].yield[i];
        bonus = resource_bonus(deco, i);
        if (bonus < 0)
            amount <<= 1;
        else
            amount += bonus;
        if (g_center_amount < amount) {
            g_center_kind = i;
            g_center_amount = amount;
        }
    }
    if ((int)g_center_kind >= 0) {
        if (g_game.difficulty == DIFFICULTY_DISCOVERER)
            g_center_amount++;
        g_center_amount = g_center_amount + river;
        if (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY)
            g_center_amount++;
        if (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS)
            g_center_amount++;
    }
    for (i = 0; i < 20; i++)
        g_prod[i] = 0;
    g_prod[GOODS_FOOD] += g_center_food;
    if ((int)g_center_kind >= 0)
        g_prod[g_center_kind] += g_center_amount;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 5; col++) {
            amt = compute_yield(col, row, &kind, 1);
            if (kind >= 0) {
                if (g_cur_colony_rec->jobs[get_field(col, row)] == JOB_FISHERMAN)
                    g_fish_food = g_fish_food + amt;
                g_prod[kind] += amt;
            }
        }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        bamt = compute_colonist_yield(i, &bkind);
        if (bkind >= 0)
            g_prod[bkind] += bamt;
    }
    g_prod[PROD_CROSSES]++;
    if (has_building(BUILDING_CHURCH))
        g_prod[PROD_CROSSES]++;
    if (has_building(BUILDING_CATHEDRAL))
        g_prod[PROD_CROSSES]++;
    g_bells_extra = 0;
    g_prod[PROD_BELLS]++;
    if (has_father(g_cur_colony_rec->nation, FATHER_THOMAS_JEFFERSON))
        g_prod[PROD_BELLS] += (int)((unsigned)g_prod[PROD_BELLS] >> 1);
    if (has_father(g_cur_colony_rec->nation, FATHER_THOMAS_PAINE))
        g_prod[PROD_BELLS] += (int)(g_nations[g_cur_colony_rec->nation].tax * (unsigned)g_prod[PROD_BELLS] / 100);
    g_prod[PROD_BELLS] += g_bells_extra;
    if (has_building(BUILDING_NEWSPAPER))
        g_prod[PROD_BELLS] <<= 1;
    else if (has_building(BUILDING_PRINTING_PRESS))
        g_prod[PROD_BELLS] += (int)((unsigned)g_prod[PROD_BELLS] >> 1);
    if (g_cur_colony_rec->stock[GOODS_HORSES] >= 2) {
        step = 25;
        if (has_building(BUILDING_STABLES) == 0)
            step <<= 1;
        horse_food = (g_cur_colony_rec->stock[GOODS_HORSES] + step - 1) / step * 2;
        breed = horse_food;
    } else
        breed = horse_food = 0;
    eat = g_cur_colony_rec->pop * 2;
    surplus = g_prod[GOODS_FOOD] - eat;
    surplus = max_int_1018_b34f(0, surplus);
    horse_food = min_int_1018_b329(breed, (surplus + 1) >> 1);
    cap = get_warehouse();
    used = min_int_1018_b329(horse_food,
                             *max_ptr_1018_b55a((zero = 0, &zero),
                                                (room = cap - g_cur_colony_rec->stock[GOODS_HORSES], &room)));
    eat += used;
    g_prod[GOODS_HORSES] += breed;
    g_shortfall[GOODS_HORSES] = breed - used;
    simple_cash_flow(0, eat);
    simple_cash_flow(5, g_prod[16]);
    factory_cash_flow(6, 14);
    factory_cash_flow(2, 10);
    factory_cash_flow(3, 11);
    factory_cash_flow(4, 12);
    factory_cash_flow(1, 9);
    simple_cash_flow(14, g_prod[GOODS_MUSKETS]);
}
