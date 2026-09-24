/* cell_production -- shipped at 1018:6587.
 *
 * Ported by tools/port.py from matched/game/cell_production.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern TerrainRec far g_terrain_rows[];              SEG20:0x78f8
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char g_turn_phase;                   DGROUP:0x4ba9
 *
 * The file's own Colony, NationRec52, TerrainRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_cell_colonist(col_i16 col, col_i16 row);
col_i16 colonist_at_cell(col_i16 x, col_i16 y, col_i16 *spec_out);
col_i16 colonist_specialty_get(col_i16 c);
col_i16 colony_sol_percent(void);
col_i16 count_terrain_around(col_i16 x, col_i16 y, col_i16 a, col_i16 b);
col_i16 current_colony_flag_get(col_i16 bit);
col_i16 max_int_1018_b34f(col_i16 a, col_i16 b);
col_i16 pair_score_table(col_i16 a, col_i16 b);

#include "colflag.h"
#include "control.h"
#include "building.h"
#include "terrain.h"
#include "goods.h"
#include "nations.h"
#include "fathers.h"
#include "jobs.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

/* 0x10 bytes */




extern unsigned char map_plane0_at(col_i16 x, col_i16 y);
extern unsigned char map_plane1_at(col_i16 x, col_i16 y);
extern col_i16 terrain_from_map_byte(col_i16 b);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
/* same object: 4-byte `push cs; call ` (rule 47) */









col_i16 cell_production(col_i16 col, col_i16 row, col_i16 *kind_out, col_i16 flag)
{
    register col_i16 amount = 0;
    register col_i16 kind;
    col_i16 tx;
    col_i16 ty;
    col_i16 terr;
    col_i16 spec_at;
    col_i16 waterc;
    col_i16 colonist;
    col_i16 deco;
    col_i16 expert;
    col_i16 convert;
    col_i16 spec;
    col_i16 step;
    col_i16 bonus;
    col_i16 sol;
    col_i16 tories;
    col_i16 divisor;
    col_i16 farmfish = 0;
    col_i16 unworked = 0;
    col_i16 morale;
    unsigned char mapbyte;

    *kind_out = kind = colonist_at_cell(col, row, &spec_at);
    if (kind < 0)
        goto done;
    tx = g_cur_colony_rec->x + col - 2;
    ty = g_cur_colony_rec->y + row - 2;
    mapbyte = map_plane0_at(tx, ty);
    terr = terrain_from_map_byte(mapbyte);
    deco = tile_decoration(tx, ty);
    amount = ((unsigned char *)((unsigned char *)&(g_terrain_rows[terr]) + 0x7))[kind];
    if (amount != 0) {
        if (kind >= 8) {
            waterc = count_terrain_around(tx, ty, TERRAIN_OCEAN, TERRAIN_SEA_LANE);
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
            if (map_plane1_at(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD))
                amount++;
            if (mapbyte & MAP0_RIVER) {
                amount++;
                if (mapbyte & MAP0_MAJOR_RIVER)
                    amount++;
            }
        }
    }
    amount = max_int_1018_b34f(0, amount);
    colonist = colony_cell_colonist(col, row);
    spec = colonist_specialty_get(colonist);
    expert = spec == kind ? 1 : 0;
    convert = spec == JOB_CONVERT ? 1 : 0;
    farmfish = kind == JOB_FARMER || kind == JOB_FISHERMAN ? 1 : 0;
    sol = colony_sol_percent();
    tories = (g_cur_colony_rec->pop * (100 - sol) + 50) / 100;
    divisor = g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_colony_rec->nation].control == CONTROL_HUMAN
              ? 10 - g_difficulty : 10;
    if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
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
    bonus = pair_score_table(deco, *kind_out);
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
            g_turn_phase++;
        if (*kind_out == GOODS_SILVER)
            g_turn_phase += 2;
    }
    if (deco == 12 && *kind_out == GOODS_SILVER)
        g_turn_phase++;
    if (*kind_out == GOODS_SILVER && deco == -1) {
        if (!(map_plane1_at(tx, ty) & MAP1_DEPLETED))
            unworked = 1;
        if (*kind_out == GOODS_SILVER && !(map_plane1_at(tx, ty) & MAP1_DEPLETED) && amount != 0)
            amount = (map_plane1_at(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)) || expert != 0 ? 1 : 0;
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
        if ((map_plane1_at(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)) && kind > GOODS_COTTON)
            bonus += step;
        if ((map_plane1_at(tx, ty) & MAP1_PLOWED) && kind <= GOODS_COTTON)
            bonus += step;
        if (mapbyte & MAP0_RIVER) {
            bonus += step;
            if ((mapbyte & MAP0_MAJOR_RIVER) && bonus == step)
                bonus += step;
        }
        amount += bonus;
    }
    if (kind >= 8 && current_colony_flag_get(BUILDING_DOCKS) == 0)
        amount = 0;
    if (kind == GOODS_FURS && nation_flag_test(g_cur_colony_rec->nation, FATHER_HENRY_HUDSON) != 0)
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
