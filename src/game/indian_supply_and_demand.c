/* PORT: this file's own declarations of write_in_color removed: the header declares it from the definition (W1 step 3). */
/* indian_supply_and_demand: candidate for 1030:ac7a (2439 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2439, the census figure. The gap to `tribe_heresy`'s neighbour is
 * 2468, but grading at 2440 already differs, so the last 29 bytes are not this function's.
 *
 * WHAT A NATIVE VILLAGE WILL BUY AND SELL. It fills the two sixteen-entry tables at SEG25:0x10 (the
 * price the village pays) and SEG25:0x30 (the price it asks), one entry per goods, for the settlement
 * and tribe currently selected. `negotiate_tribe`, `1030:b61e`, `1030:c915` and `1030:d2aa` call
 * it before showing a trade.
 *
 * FIRST it marks, in a 5x5 grid, which of the squares round the village a European colony is already
 * working -- every colony in the game is selected in turn and its own 5x5 compared against the
 * village's. Those squares are then skipped.
 *
 * THEN it walks the village's own 5x5 and tallies the land: mountains and hills separately, forests
 * split into a cold band (`(t & 7) < 3`) and a warm one, and running totals for cold ground, warm
 * ground, food yield, and the three cash crops. Each ocean square adds the tribe's
 * level plus one to a counter, and every three in the counter add two to the food yield.
 *
 * FROM THAT, every price. Food is the village's own size squared times four, halved for a tribe above
 * level 1; furs come from the forests divided by the tribe's level; coats and cloth are worked out
 * from the furs and cotton the village already has; ore and silver need level 1 and 2 respectively,
 * silver scaled by how many settlements the tribe has; horses come off the tribe's own herd. The
 * prices the village PAYS are then clamped to 0..50.
 *
 * A CAPITAL (settlement flag bit 2) doubles what it pays for goods 0..7, adds half again for 13..15,
 * and doubles what it asks for 7..15.
 *
 * FINALLY, per goods: the tribe's own drift at Tribe+0xe (positive means it already has plenty) takes
 * `(drift + 50) / 100 * 2` off the buying price or adds it to the asking price; each side is then
 * reduced by half the other, so a village that wants a thing dearly does not also sell it cheaply;
 * and either side that started positive is floored at 1. Debug bit 2 prints every non-zero pair with
 * "%Fs %d %d\n".
 *
 * WHAT THE BYTES SETTLED: `(size * size * 4) >> (level > 1)` only reproduces with the shift count at
 * CHAR rank -- `level > 1 ? (char)1 : (char)0` -- because Borland builds the flag in AL and moves it
 * straight to CL. Every doubling is `x * 2` (ADD AX,AX) and not `x + x`, which would re-read the slot.
 * The `level >= 1` test appears twice, once nested inside itself, which is how the shipped jumps run.
 * And the three current-record pointers are mixed: the settlement and tribe are in this module,
 * the colony .
 *
 * RENAMED 2026-09-26 from native_trade_prices: the Mac build's indian_supply_and_demand(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee minimax; callee on_map; callee set_colony; callee strings; callee terrain_at), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "dbgopt.h"
#include "tribes.h"
#include "goods.h"
#include "settle.h"
#include "terrain.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);

extern char get_field(int col, int row);

extern char *strings(int n);


/* same object: 4-byte `push cs; call ` (rule 47) */

void indian_supply_and_demand(void)
{
    int row;
    int cx;
    int cy;
    int band;
    int i;
    int idx;
    int mountains;
    int hills;
    int cold_forest;
    int deep;
    int cold;
    int warm;
    int food_yield;
    int sugar;
    int sea;
    int cotton;
    int tobacco;
    int ore;
    int size1;
    int size2;
    int keep;
    int bonus;
    int drift;
    int settlements;
    int sell0;
    int buy0;
    char grid[25];
    char buf[80];
    register int terrain;
    register int col;

    band = 0;
    mountains = 0;
    hills = 0;
    cold_forest = 0;
    deep = 0;
    cold = 0;
    warm = 0;
    food_yield = 0;
    sugar = 0;
    sea = 0;
    cotton = 0;
    tobacco = 0;
    ore = 0;
    for (i = 0; i < 0x19; i++)
        grid[i] = 0;
    for (i = 0; i < g_game.colony_count; i++) {
        set_colony(i);
        for (row = 0; row < 5; row++) {
            for (col = 0; col < 5; col++) {
                cx = g_cur_settlement_rec->x + col - g_cur_colony_rec->x;
                cy = g_cur_settlement_rec->y + row - g_cur_colony_rec->y;
                if (on_map(cx - 2, cy - 2) && cx >= 0 && cx < 5 && cy >= 0 && cy < 5
                    && ((col == 2 && row == 2)
                        || (int)get_field(col, row) >= 0)) {
                    idx = row * 5 + col;
                    grid[idx] = 1;
                }
            }
        }
    }
    for (row = g_cur_settlement_rec->y - 2; row <= g_cur_settlement_rec->y + 2; row++) {
        for (col = g_cur_settlement_rec->x - 2; col <= g_cur_settlement_rec->x + 2; col++) {
            if (on_map(col, row)) {
                cx = col - g_cur_settlement_rec->x + 2;
                cy = row - g_cur_settlement_rec->y + 2;
                idx = cy * 5 + cx;
                if (grid[idx] == 0) {
                    terrain = terrain_at(col, row);
                    if (terrain == TERRAIN_MOUNTAINS)
                        mountains++;
                    if (terrain == TERRAIN_HILLS)
                        hills++;
                    if (terrain == TERRAIN_ARCTIC)
                        cold += 4;
                    if ((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)) {
                        food_yield = food_yield + 1;
                        if (terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST)
                            band = terrain - 8;
                        if (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)
                            band = terrain - 0x10;
                        if (band >= 3) {
                            deep++;
                            warm++;
                            if (band == 5)
                                sugar += 2;
                            if (band == 4)
                                tobacco += 2;
                            if (band == 3)
                                cotton += 2;
                        } else {
                            cold_forest++;
                            cold += 2;
                        }
                    } else if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) {
                        sea = g_cur_tribe_rec->level + sea + 1;
                        while (sea >= 3) {
                            food_yield = food_yield + 2;
                            sea = sea + -3;
                        }
                    } else if (terrain < TERRAIN_FOREST_FIRST) {
                        if (terrain == TERRAIN_SAVANNAH)
                            sugar += 4;
                        if (terrain == TERRAIN_SWAMP)
                            sugar += 2;
                        if (terrain == TERRAIN_GRASSLAND)
                            tobacco += 4;
                        if (terrain == TERRAIN_MARSH)
                            tobacco += 2;
                        if (terrain == TERRAIN_PRAIRIE)
                            cotton += 4;
                        if (terrain == TERRAIN_TUNDRA)
                            ore += 2;
                        if (terrain == TERRAIN_PLAINS) {
                            cotton++;
                            food_yield += 2;
                        }
                        if (terrain > TERRAIN_DESERT) {
                            food_yield += 2;
                            if (terrain < TERRAIN_MARSH) {
                                food_yield++;
                                if (terrain & 4)
                                    warm += 2;
                                else
                                    cold += 2;
                            } else
                                ore++;
                        } else if (terrain == TERRAIN_DESERT)
                            warm += 4;
                        else if (terrain == TERRAIN_TUNDRA)
                            cold += 3;
                    }
                }
            }
        }
    }
    size1 = g_cur_settlement_rec->size + 1;
    size2 = size1 * size1;
    for (i = 0; i < GOODS_COUNT; i++)
        g_native_sell[i] = g_native_buy[i] = 0;
    g_native_sell[GOODS_FOOD] += (g_cur_tribe_rec->level + size1) * food_yield
                        / (7 - g_cur_tribe_rec->level);
    g_native_buy[GOODS_FOOD] = (size2 << 2) >> (g_cur_tribe_rec->level > LEVEL_AGRARIAN ? (char)1 : (char)0);
    if (g_cur_tribe_rec->level >= LEVEL_AGRARIAN) {
        if (g_cur_tribe_rec->level >= LEVEL_ADVANCED) {
            settlements = max_int_1030_ead6(1, g_tribe_settlement_count[g_cur_tribe]);
            g_native_sell[GOODS_SILVER] = g_cur_tribe_rec->silver_base / settlements;
            bonus = mountains << 2;
            if (g_cur_tribe_rec->level > LEVEL_ADVANCED)
                bonus = bonus * 2;
            g_native_sell[GOODS_SILVER] += bonus;
        }
        if (g_cur_tribe_rec->level >= LEVEL_AGRARIAN)
            g_native_sell[GOODS_ORE] += hills * 2 + mountains + ore;
    }
    g_native_sell[GOODS_FURS] += (cold_forest * 2 + (deep >> 1)) / (g_cur_tribe_rec->level + 1);
    g_native_sell[GOODS_COATS] = (g_cur_tribe_rec->level + g_native_sell[GOODS_FURS]) * 2 / 4 * 2;
    g_native_sell[GOODS_COTTON] += cotton;
    g_native_sell[GOODS_TOBACCO] += tobacco;
    g_native_sell[GOODS_SUGAR] += sugar;
    g_native_buy[GOODS_CLOTH] = (g_cur_tribe_rec->level + size1) * size1 + (warm >> 1) + cold;
    g_native_sell[GOODS_CLOTH] = (g_cur_tribe_rec->level + g_native_sell[GOODS_COTTON]) * 2 / 4 * 2;
    g_native_buy[GOODS_COTTON] = g_native_sell[GOODS_CLOTH] + warm;
    g_native_buy[GOODS_TOBACCO] = (5 - g_cur_tribe_rec->level + 1) * size1 + cold * 2 + 5;
    g_native_buy[GOODS_CIGARS] = (size1 << 2) + (5 - g_cur_tribe_rec->level) * 2 + 4;
    g_native_buy[GOODS_COATS] = (cold << 3) + g_native_sell[GOODS_FURS];
    g_native_buy[GOODS_RUM] = (size1 << 2) + (g_cur_tribe_rec->level << 3) + warm * 2;
    g_native_buy[GOODS_TRADE_GOODS] = (g_cur_tribe_rec->level + 2) * (size1 + 3) + 8;
    g_native_buy[GOODS_TOOLS] = (g_cur_tribe_rec->level * size1) << ((cold >> 1) + 1);
    g_native_sell[GOODS_MUSKETS] = 0;
    g_native_buy[GOODS_MUSKETS] = (5 - g_cur_tribe_rec->level + 2 - (int)g_cur_tribe_rec->muskets) << 2;
    g_native_sell[GOODS_HORSES] = g_cur_tribe_rec->horses
                       / ((g_tribe_settlement_count[g_cur_tribe] >> 1) + 1);
    g_native_buy[GOODS_HORSES] = (5 - g_cur_tribe_rec->level + 4 - (int)g_cur_tribe_rec->horse_herds) << 2;
    for (i = 0; i < GOODS_COUNT; i++)
        g_native_buy[i] = minimax(g_native_buy[i], 0, 0x32);
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL) {
        for (i = 0; i <= 7; i++)
            g_native_buy[i] = g_native_buy[i] * 2;
        for (i = 0xd; i <= 0xf; i++)
            g_native_buy[i] += g_native_buy[i] >> 1;
        for (i = 7; i < GOODS_COUNT; i++)
            g_native_sell[i] = g_native_sell[i] * 2;
    }
    for (i = 0; i < GOODS_COUNT; i++) {
        sell0 = g_native_sell[i];
        buy0 = g_native_buy[i];
        drift = g_cur_tribe_rec->goods_drift[i];
        if (drift > 0)
            g_native_buy[i] -= (drift + 0x32) / 100 * 2;
        else if (drift < 0)
            g_native_sell[i] += (drift + 0x32) / 100 * 2;
        if ((g_debug_flags & DBG_SUPPLY_AND_DEMAND_INDIANS) && (g_native_sell[i] != 0 || g_native_buy[i] != 0)) {
            sprintf_rtl(buf, "%Fs %d %d\n", strings(g_goods_name_word[i]),
                        g_native_sell[i], g_native_buy[i]);
            write_in_color(buf, 1, i * 8 + 8, 0xf);
        }
        keep = g_native_sell[i];
        g_native_sell[i] = g_native_sell[i] - (g_native_buy[i] >> 1);
        g_native_buy[i] = g_native_buy[i] - (keep >> 1);
        g_native_sell[i] = max_int_1030_ead6(sell0 > 0, g_native_sell[i]);
        g_native_buy[i] = max_int_1030_ead6(buy0 > 0, g_native_buy[i]);
    }
    if (g_debug_flags & DBG_SUPPLY_AND_DEMAND_INDIANS)
        ;
}
