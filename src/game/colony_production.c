/* colony_production -- shipped at 1018:544c.
 *
 * Ported by tools/port.py from matched/game/colony_production.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern unsigned char g_center_food;                  DGROUP:0x4ba4
 *   extern unsigned char g_4ba5;                         DGROUP:0x4ba5
 *   extern signed char g_center_kind;                    DGROUP:0x4ba6
 *   extern signed char g_center_amount;                  DGROUP:0x4ba7
 *   extern unsigned char g_fish_food;                    DGROUP:0x4ba8
 *   extern unsigned char g_turn_phase;                   DGROUP:0x4ba9
 *   extern unsigned char far g_yield[][16];              SEG20:0x78ff
 *   extern unsigned int far g_prod[];                    SEG21:0x0122
 *   extern int far g_tbl21_1c4;                          SEG21:0x01c4
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Colony, Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_cell_colonist(col_i16 col, col_i16 row);
col_i16 colony_warehouse_capacity(void);
col_i16 current_colony_flag_get(col_i16 bit);
col_i16 max_int_1018_b34f(col_i16 a, col_i16 b);
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);
void set_slot_from_table(col_i16 slot, col_i16 index);
void set_slot_value(col_i16 slot, col_i16 v);

#include "difficul.h"
#include "colflag.h"
#include "building.h"
#include "terrain.h"
#include "goods.h"
#include "jobs.h"
#include "fathers.h"
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern unsigned char map_plane1_at(col_i16 x, col_i16 y);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);
extern col_i16 pair_score_table(col_i16 a, col_i16 b);
extern col_i16 cell_production(col_i16 col, col_i16 row, col_i16 *kind_out, col_i16 flag);
extern col_i16 building_output(col_i16 i, col_i16 *kind_out);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
extern col_i16 * max_ptr_1018_b55a(col_i16 *a, col_i16 *b);
/* same object: 4-byte `push cs; call ` (rule 47) */








void colony_production(void)
{
    col_i16 i;
    col_i16 amt;
    col_i16 kind;
    col_i16 bamt;
    col_i16 bkind;
    col_i16 t;
    col_i16 v;
    col_i16 horse_food;
    col_i16 breed;
    col_i16 used;
    col_i16 eat;
    col_i16 surplus;
    col_i16 deco;
    col_i16 river;
    col_i16 p0;
    col_i16 cap;
    col_i16 bonus;
    col_i16 step;
    col_i16 room;
    col_i16 zero;
    register col_i16 col;
    register col_i16 row;

    g_fish_food = 0;
    g_turn_phase = 0;
    t = terrain_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (t == TERRAIN_ARCTIC)
        g_center_food = 0;
    else if (t == TERRAIN_DESERT || t == TERRAIN_SCRUB_FOREST_2 || t == TERRAIN_SCRUB_FOREST)
        g_center_food = 1;
    else if (t == TERRAIN_MOUNTAINS || t == TERRAIN_HILLS
             || (t >= TERRAIN_BOREAL_FOREST && t < TERRAIN_BOREAL_FOREST_2)
             || (t >= TERRAIN_BOREAL_FOREST_2 && t < TERRAIN_ARCTIC))
        g_center_food = 2;
    else
        g_center_food = 3;
    if (g_difficulty == DIFFICULTY_DISCOVERER)
        g_center_food = g_center_food + 2;
    if (g_difficulty == DIFFICULTY_EXPLORER)
        g_center_food++;
    if (map_plane1_at(g_cur_colony_rec->x, g_cur_colony_rec->y) & MAP1_PLOWED)
        g_center_food++;
    deco = tile_decoration(g_cur_colony_rec->x, g_cur_colony_rec->y);
    river = 0;
    p0 = map_plane0_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
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
        v = g_yield[t][i];
        bonus = pair_score_table(deco, i);
        if (bonus < 0)
            v <<= 1;
        else
            v += bonus;
        if (g_center_amount < v) {
            g_center_kind = i;
            g_center_amount = v;
        }
    }
    if ((col_i16)g_center_kind >= 0) {
        if (g_difficulty == DIFFICULTY_DISCOVERER)
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
    if ((col_i16)g_center_kind >= 0)
        g_prod[g_center_kind] += g_center_amount;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 5; col++) {
            amt = cell_production(col, row, &kind, 1);
            if (kind >= 0) {
                if (((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[colony_cell_colonist(col, row)] == JOB_FISHERMAN)
                    g_fish_food = g_fish_food + amt;
                g_prod[kind] += amt;
            }
        }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        bamt = building_output(i, &bkind);
        if (bkind >= 0)
            g_prod[bkind] += bamt;
    }
    g_prod[PROD_CROSSES]++;
    if (current_colony_flag_get(BUILDING_CHURCH))
        g_prod[PROD_CROSSES]++;
    if (current_colony_flag_get(BUILDING_CATHEDRAL))
        g_prod[PROD_CROSSES]++;
    g_4ba5 = 0;
    g_prod[PROD_BELLS]++;
    if (nation_flag_test(g_cur_colony_rec->nation, FATHER_THOMAS_JEFFERSON))
        g_prod[PROD_BELLS] += g_prod[PROD_BELLS] >> 1;
    if (nation_flag_test(g_cur_colony_rec->nation, FATHER_THOMAS_PAINE))
        g_prod[PROD_BELLS] += g_nations[g_cur_colony_rec->nation].tax * g_prod[PROD_BELLS] / 100;
    g_prod[PROD_BELLS] += g_4ba5;
    if (current_colony_flag_get(BUILDING_NEWSPAPER))
        g_prod[PROD_BELLS] <<= 1;
    else if (current_colony_flag_get(BUILDING_PRINTING_PRESS))
        g_prod[PROD_BELLS] += g_prod[PROD_BELLS] >> 1;
    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[GOODS_HORSES] >= 2) {
        step = 25;
        if (current_colony_flag_get(BUILDING_STABLES) == 0)
            step <<= 1;
        horse_food = (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[GOODS_HORSES] + step - 1) / step * 2;
        breed = horse_food;
    } else
        breed = horse_food = 0;
    eat = g_cur_colony_rec->pop * 2;
    surplus = g_prod[GOODS_FOOD] - eat;
    surplus = max_int_1018_b34f(0, surplus);
    horse_food = min_int_1018_b329(breed, (surplus + 1) >> 1);
    cap = colony_warehouse_capacity();
    used = min_int_1018_b329(horse_food,
                             *max_ptr_1018_b55a((zero = 0, &zero),
                                                (room = cap - ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[GOODS_HORSES], &room)));
    eat += used;
    g_prod[GOODS_HORSES] += breed;
    g_tbl21_1c4 = breed - used;
    set_slot_value(0, eat);
    set_slot_value(5, g_prod[16]);
    set_slot_from_table(6, 14);
    set_slot_from_table(2, 10);
    set_slot_from_table(3, 11);
    set_slot_from_table(4, 12);
    set_slot_from_table(1, 9);
    set_slot_value(14, g_prod[GOODS_MUSKETS]);
}
