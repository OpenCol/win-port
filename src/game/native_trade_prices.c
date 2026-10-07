/* native_trade_prices -- shipped at 1030:ac7a.
 *
 * Ported by tools/port.py from matched/game/native_trade_prices.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern Settlement far *g_cur_settlement_rec;         DGROUP:0x4c2c
 *   extern Tribe far *g_cur_tribe_rec;                   DGROUP:0x4c28
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *   extern int far g_debug_flags;                        DGROUP:0x00b6
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern unsigned char far g_tribe_settlement_count[]; SEG20:0xdc10
 *   extern int far g_native_buy[];                       SEG25:0x0010
 *   extern int far g_native_sell[];                      SEG25:0x0030
 *
 * The file's own Colony, Settlement, Tribe dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: silver_base -> fc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1030_ead6(col_i16 a, col_i16 b);

#include "dbgopt.h"
#include "tribes.h"
#include "goods.h"
#include "settle.h"
#include "terrain.h"
extern void set_current_colony(col_i16 c);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern char colony_cell_colonist(col_i16 col, col_i16 row);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 minimax(col_i16 a, col_i16 lo, col_i16 hi);
extern char * nth_text_string(col_i16 n);
extern col_i16 sprintf_rtl(char *buf, const char *fmt, ...);
extern col_i16 return_arg2_1040_a4f4(char *s, col_i16 x, col_i16 y, col_i16 c);

/* same object: 4-byte `push cs; call ` (rule 47) */


void native_trade_prices(void)
{
    col_i16 row;
    col_i16 cx;
    col_i16 cy;
    col_i16 band;
    col_i16 i;
    col_i16 idx;
    col_i16 mountains;
    col_i16 hills;
    col_i16 open;
    col_i16 deep;
    col_i16 food;
    col_i16 cloth_in;
    col_i16 lumber;
    col_i16 sugar;
    col_i16 sea;
    col_i16 cotton;
    col_i16 tobacco;
    col_i16 ore;
    col_i16 size1;
    col_i16 size2;
    col_i16 keep;
    col_i16 bonus;
    col_i16 drift;
    col_i16 level;
    col_i16 sell0;
    col_i16 buy0;
    char grid[25];
    char buf[80];
    register col_i16 terrain;
    register col_i16 col;

    band = 0;
    mountains = 0;
    hills = 0;
    open = 0;
    deep = 0;
    food = 0;
    cloth_in = 0;
    lumber = 0;
    sugar = 0;
    sea = 0;
    cotton = 0;
    tobacco = 0;
    ore = 0;
    for (i = 0; i < 0x19; i++)
        grid[i] = 0;
    for (i = 0; i < g_colony_count; i++) {
        set_current_colony(i);
        for (row = 0; row < 5; row++) {
            for (col = 0; col < 5; col++) {
                cx = g_cur_settlement_rec->x + col - g_cur_colony_rec->x;
                cy = g_cur_settlement_rec->y + row - g_cur_colony_rec->y;
                if (on_map(cx - 2, cy - 2) && cx >= 0 && cx < 5 && cy >= 0 && cy < 5
                    && ((col == 2 && row == 2)
                        || (col_i16)colony_cell_colonist(col, row) >= 0)) {
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
                        food += 4;
                    if ((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)) {
                        lumber = lumber + 1;
                        if (terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST)
                            band = terrain - 8;
                        if (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)
                            band = terrain - 0x10;
                        if (band >= 3) {
                            deep++;
                            cloth_in++;
                            if (band == 5)
                                sugar += 2;
                            if (band == 4)
                                tobacco += 2;
                            if (band == 3)
                                cotton += 2;
                        } else {
                            open++;
                            food += 2;
                        }
                    } else if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) {
                        sea = g_cur_tribe_rec->level + sea + 1;
                        while (sea >= 3) {
                            lumber = lumber + 2;
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
                            lumber += 2;
                        }
                        if (terrain > TERRAIN_DESERT) {
                            lumber += 2;
                            if (terrain < TERRAIN_MARSH) {
                                lumber++;
                                if (terrain & 4)
                                    cloth_in += 2;
                                else
                                    food += 2;
                            } else
                                ore++;
                        } else if (terrain == TERRAIN_DESERT)
                            cloth_in += 4;
                        else if (terrain == TERRAIN_TUNDRA)
                            food += 3;
                    }
                }
            }
        }
    }
    size1 = g_cur_settlement_rec->size + 1;
    size2 = size1 * size1;
    for (i = 0; i < GOODS_COUNT; i++)
        g_native_sell[i] = g_native_buy[i] = 0;
    g_native_sell[GOODS_FOOD] += (g_cur_tribe_rec->level + size1) * lumber
                        / (7 - g_cur_tribe_rec->level);
    g_native_buy[GOODS_FOOD] = (size2 << 2) >> (g_cur_tribe_rec->level > LEVEL_AGRARIAN ? (char)1 : (char)0);
    if (g_cur_tribe_rec->level >= LEVEL_AGRARIAN) {
        if (g_cur_tribe_rec->level >= LEVEL_ADVANCED) {
            level = max_int_1030_ead6(1, g_tribe_settlement_count[g_cur_tribe]);
            g_native_sell[GOODS_SILVER] = g_cur_tribe_rec->fc / level;
            bonus = mountains << 2;
            if (g_cur_tribe_rec->level > LEVEL_ADVANCED)
                bonus = bonus * 2;
            g_native_sell[GOODS_SILVER] += bonus;
        }
        if (g_cur_tribe_rec->level >= LEVEL_AGRARIAN)
            g_native_sell[GOODS_ORE] += hills * 2 + mountains + ore;
    }
    g_native_sell[GOODS_FURS] += (open * 2 + (deep >> 1)) / (g_cur_tribe_rec->level + 1);
    g_native_sell[GOODS_COATS] = (g_cur_tribe_rec->level + g_native_sell[GOODS_FURS]) * 2 / 4 * 2;
    g_native_sell[GOODS_COTTON] += cotton;
    g_native_sell[GOODS_TOBACCO] += tobacco;
    g_native_sell[GOODS_SUGAR] += sugar;
    g_native_buy[GOODS_CLOTH] = (g_cur_tribe_rec->level + size1) * size1 + (cloth_in >> 1) + food;
    g_native_sell[GOODS_CLOTH] = (g_cur_tribe_rec->level + g_native_sell[GOODS_COTTON]) * 2 / 4 * 2;
    g_native_buy[GOODS_COTTON] = g_native_sell[GOODS_CLOTH] + cloth_in;
    g_native_buy[GOODS_TOBACCO] = (5 - g_cur_tribe_rec->level + 1) * size1 + food * 2 + 5;
    g_native_buy[GOODS_CIGARS] = (size1 << 2) + (5 - g_cur_tribe_rec->level) * 2 + 4;
    g_native_buy[GOODS_COATS] = (food << 3) + g_native_sell[GOODS_FURS];
    g_native_buy[GOODS_RUM] = (size1 << 2) + (g_cur_tribe_rec->level << 3) + cloth_in * 2;
    g_native_buy[GOODS_TRADE_GOODS] = (g_cur_tribe_rec->level + 2) * (size1 + 3) + 8;
    g_native_buy[GOODS_TOOLS] = (g_cur_tribe_rec->level * size1) << ((food >> 1) + 1);
    g_native_sell[GOODS_MUSKETS] = 0;
    g_native_buy[GOODS_MUSKETS] = (5 - g_cur_tribe_rec->level + 2 - (col_i16)g_cur_tribe_rec->muskets) << 2;
    g_native_sell[GOODS_HORSES] = g_cur_tribe_rec->horses
                       / ((g_tribe_settlement_count[g_cur_tribe] >> 1) + 1);
    g_native_buy[GOODS_HORSES] = (5 - g_cur_tribe_rec->level + 4 - (col_i16)g_cur_tribe_rec->horse_taken) << 2;
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
        drift = ((col_i16 *)((unsigned char *)(g_cur_tribe_rec) + 0xe))[i];
        if (drift > 0)
            g_native_buy[i] -= (drift + 0x32) / 100 * 2;
        else if (drift < 0)
            g_native_sell[i] += (drift + 0x32) / 100 * 2;
        if ((g_debug_flags & DBG_SUPPLY_AND_DEMAND_INDIANS) && (g_native_sell[i] != 0 || g_native_buy[i] != 0)) {
            sprintf_rtl(buf, "%Fs %d %d\n", nth_text_string(g_goods_name_word[i]),
                        g_native_sell[i], g_native_buy[i]);
            return_arg2_1040_a4f4(buf, 1, i * 8 + 8, 0xf);
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
