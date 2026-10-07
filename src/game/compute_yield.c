/* compute_yield: candidate for 1018:6587 (1177 bytes).  Verdict: RELOCATABLE.
 * symbol: compute_yield
 * name-source: sibling-build
 * size-source: 1177, not the census 1168. The trailing 9 bytes are the dead closing-brace epilogue
 * after `done: return amount;`; compute_colonist_yield starts immediately after.
 *
 * THE GAME'S OUTDOOR PRODUCTION FORMULA: how much the colonist on square (col, row) of the current
 * colony's work area makes, and of what. `*kind_out` receives the production kind -- goods 0..7, and
 * 8 and above for the catch from water -- which `get_yield_type` decides.
 *
 *   1. BASE: the yield table at SEG20:0x78ff, sixteen columns per terrain id.
 *   2. WATER (kind >= 8), by how many of the eight neighbours are TERRAIN_OCEAN or TERRAIN_SEA_LANE:
 *      8 costs 2, 6 or 7 cost 1, fewer adds 1. THREE MORE ARMS ARE DEAD -- `< 4` adds 2, `< 3` adds 3,
 *      `< 1` adds 4 -- because they follow a `< 6` arm that already caught every such count. The
 *      bigger coastal bonuses were written and never reached.
 *   3. FURS: +1 on a road or settlement square, +1 for a river, +1 more for a major one.
 *   4. REBELS: tories = population x (100 - rebel%) / 100, rounded; the penalty is tories / (10 -
 *      difficulty) for a human European colony and nothing for anyone else, and colony flags 4 and 2
 *      (+0x1c) each take one off it. A net bonus is added to any non-zero output; a net penalty is
 *      applied last.
 *   5. EXPERTS (specialty == kind): farmers and fishermen get +2 (and the rebel bonus again), everyone
 *      else doubles.
 *   6. SPECIAL RESOURCES (`resource_at`, scored by `resource_bonus`): a negative score
 *      doubles, a positive one is added -- doubled for an expert. Ore and silver on resource squares
 *      6 and 12 also count toward DGROUP:0x4ba9.
 *   7. SILVER on a square with no resource and plane-1 bit 2 clear makes at most 1, and only with a
 *      road, a settlement or an expert.
 *   8. LUMBER doubles. Then +step for food, +step for a road or settlement (manufactured-side kinds, 4
 *      and up), +step for plowed land (the crops, 0..3), +step for a river and +step more for a major
 *      river when nothing else applied -- step 2 for lumber and non-farming experts, else 1.
 *   9. FISH needs colony building 6, BUILDING6 "DOCKS"; FURS double with FATHER_HENRY_HUDSON; and a
 *      colonist whose specialty is 0x1b, JOB27 "INDIAN CONVERTS", adds 1 to food, sugar, tobacco,
 *      cotton, furs and fish -- all three the game's documented rules.
 *
 * With `flag`, water production is reported as kind 0, food. `*kind_out = kind = ...` is one chained
 * store (the shipped writes AX, not DI, into *kind_out).
 *
 * RENAMED 2026-09-26 from cell_production: the Mac build's compute_yield(short, short, short*, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller DrawColonyMap; caller colony_select_profession; callee colony_rebel; callee feature_get; callee get_specialty), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "colflag.h"
#include "control.h"
#include "building.h"
#include "terrain.h"
#include "goods.h"
#include "nations.h"
#include "fathers.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern int terrain_type(int map_byte);
extern int resource_at(int x, int y);
extern int has_father(int nation, int bit);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_yield_type(int x, int y, int *spec_out);
extern int scan_for_terrain(int x, int y, unsigned char a, unsigned char b);

extern char get_field(int col, int row);
extern int get_specialty(int u);
extern int colony_rebel(void);
extern int resource_bonus(int resource, int goods);
extern int has_building(int bit);

int compute_yield(int col, int row, int *kind_out, int flag)
{
    register int amount = 0;
    register int kind;
    int tx;
    int ty;
    int terr;
    int spec_at;
    int waterc;
    int colonist;
    int deco;
    int expert;
    int convert;
    int spec;
    int step;
    int bonus;
    int sol;
    int tories;
    int divisor;
    int farmfish = 0;
    int unworked = 0;
    int morale;
    unsigned char mapbyte;

    *kind_out = kind = get_yield_type(col, row, &spec_at);
    if (kind < 0)
        goto done;
    tx = g_cur_colony_rec->x + col - 2;
    ty = g_cur_colony_rec->y + row - 2;
    mapbyte = map_get(tx, ty);
    terr = terrain_type(mapbyte);
    deco = resource_at(tx, ty);
    amount = g_terrain_rows[terr].yield[kind];
    if (amount != 0) {
        if (kind >= 8) {
            waterc = scan_for_terrain(tx, ty, TERRAIN_OCEAN, TERRAIN_SEA_LANE);
            if (waterc >= 8)
                amount -= 2;
            else if (waterc >= 6)
                amount--;
            else if (waterc < 6)
                amount++;
            else if (waterc < 4)
                amount += 2;
            else if (waterc < 3)
                amount += 3;
            else if (waterc < 1)
                amount += 4;
        }
        if (kind == GOODS_FURS) {
            if (feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD))
                amount++;
            if (mapbyte & MAP0_RIVER) {
                amount++;
                if (mapbyte & MAP0_MAJOR_RIVER)
                    amount++;
            }
        }
    }
    amount = max_int_1018_b34f(0, amount);
    colonist = get_field(col, row);
    spec = get_specialty(colonist);
    expert = spec == kind ? 1 : 0;
    convert = spec == JOB_CONVERT ? 1 : 0;
    farmfish = kind == JOB_FARMER || kind == JOB_FISHERMAN ? 1 : 0;
    sol = colony_rebel();
    tories = (g_cur_colony_rec->pop * (100 - sol) + 50) / 100;
    divisor = g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_colony_rec->nation].control == CONTROL_HUMAN
              ? 10 - g_game.difficulty : 10;
    if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
        tories = 0;
    morale = -(tories / divisor);
    if (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY)
        morale++;
    if (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS)
        morale++;
    if (amount != 0 && morale > 0)
        amount += morale;
    if (expert != 0) {
        if (amount != 0) {
            if (farmfish != 0) {
                amount += 2;
                if (morale > 0)
                    amount += morale;
            } else
                amount = amount * 2;
        }
    } else {
    }
    bonus = resource_bonus(deco, *kind_out);
    if (deco == 7 && amount <= 0)
        bonus = 0;
    if (bonus < 0)
        amount = amount * 2;
    else {
        if (expert != 0)
            bonus = bonus * 2;
        amount += bonus;
    }
    if (deco == 6) {
        if (*kind_out == GOODS_ORE)
            g_depletion_rolls++;
        if (*kind_out == GOODS_SILVER)
            g_depletion_rolls += 2;
    }
    if (deco == 12 && *kind_out == GOODS_SILVER)
        g_depletion_rolls++;
    if (*kind_out == GOODS_SILVER && deco == -1) {
        if (!(feature_get(tx, ty) & MAP1_DEPLETED))
            unworked = 1;
        if (*kind_out == GOODS_SILVER && !(feature_get(tx, ty) & MAP1_DEPLETED) && amount != 0)
            amount = (feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)) || expert != 0 ? 1 : 0;
    }
    if (kind == GOODS_LUMBER)
        amount = amount * 2;
    if (amount > 0 && unworked == 0) {
        step = 1;
        if ((expert != 0 && farmfish == 0) || kind == GOODS_LUMBER)
            step = 2;
        bonus = 0;
        if (kind == GOODS_FOOD)
            bonus += step;
        if ((feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)) && kind > GOODS_COTTON)
            bonus += step;
        if ((feature_get(tx, ty) & MAP1_PLOWED) && kind <= GOODS_COTTON)
            bonus += step;
        if (mapbyte & MAP0_RIVER) {
            bonus += step;
            if ((mapbyte & MAP0_MAJOR_RIVER) && bonus == step)
                bonus += step;
        }
        amount += bonus;
    }
    if (kind >= 8 && has_building(BUILDING_DOCKS) == 0)
        amount = 0;
    if (kind == GOODS_FURS && has_father(g_cur_colony_rec->nation, FATHER_HENRY_HUDSON) != 0)
        amount += amount;
    if (convert != 0 && amount >= 0
        && (kind == GOODS_FOOD || kind == GOODS_TOBACCO || kind == GOODS_COTTON || kind == GOODS_SUGAR
            || kind == GOODS_FURS || kind >= 8))
        amount++;
    amount = max_int_1018_b34f(0, amount);
    if (flag != 0 && kind >= 8)
        *kind_out = 0;
    if (amount != 0 && morale < 0) {
        amount += morale;
        amount = max_int_1018_b34f(0, amount);
    }
done:
    return amount;
}
