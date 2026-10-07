/* check_tut: candidate for 1008:1580 (2220 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2220, not the census 2196. The trailing 24 bytes are the twelve-entry jump table of
 * the resource `switch`, which Borland puts in the code segment after the epilogue; 1008:1e2c
 * starts right after them.
 *
 * THE TUTORIAL HINTS, run by `GameBrains_EndUnitTurn` for whichever unit is active
 * (SEG20:0x7794). Nine hints are tried in order and the first that fits is shown, once ever: each
 * sets its own bit in SEG20:0x7788 or 0x7782 so it never comes back. A unit with standing orders
 * gets nothing at all.
 *
 *   TUTORIAL1  turn 0, the first unit -- what the ship is carrying.
 *   TUTORIAL11 before turn 20, an empty ship with nothing in either link -- explore with it.
 *   TUTORIAL13 before turn 20, pioneers ashore for a nation with no colony yet -- find a site.
 *   TUTORIAL14 before turn 20, soldiers ashore -- what soldiers are for.
 *   TUTORIAL15 before turn 40, a colonist standing in a colony -- put him to work.
 *   TUTORIAL3  pioneers, still no colony: THIS is a good site. The square must not be water,
 *              mountains or hills, must not be next to a colony already, must touch ocean in
 *              map-plane-2 region 1, and of the nine squares around it at least five must grow food
 *              (a non-zero first column of the yield table) -- a small land region counts one more.
 *              A special resource among them names the goods the message brags about: cotton,
 *              tobacco, sugar, furs, lumber, silver or ore, with plain food and lumber only taken if
 *              nothing better turned up.
 *   TUTORIAL8  a plain colonist or an indentured servant whose nation has met a tribe -- the natives
 *              will teach him a profession.
 *   TUTORIAL9  pioneers on an unroaded forest, mountain or hill square of one of the nation's own
 *              colonies -- build a road.
 *   TUTORIAL10 pioneers on an unplowed square of their own colony -- plow or clear. It wants either
 *              open ground (terrain 2..5) or, after turn 16, a forest whose low three bits are 2..5
 *              with at least two more forests around the colony and no open ground beside it.
 *
 * WHAT THE BYTES SETTLED: the guard at the top is a `return` and compiles to its own inline
 * epilogue, while every hint ends with a `goto` to a label at the end of the function -- written as
 * `return` they would each inline an epilogue too, which is how the two are told apart. `find_colony`
 * and `find_close_colony` are separate statements inside the `if`, not assignments in
 * its condition (the shipped code re-reads the slot to test it). The two calls in the TUTORIAL3 test
 * pass the unit's own +0/+1 rather than the `x`/`y` locals holding the same values. `i` and `best`
 * are the two `register` locals, SI and DI in declaration order (rule 53), and the resource switch
 * places its arms in the order 2, 3, 4, 5, 8/9, 10, 12, 6/13 -- the shared arms last.
 *
 * RENAMED 2026-09-26 from show_tutorial_hint: the Mac build's check_tut(void), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (callee continent_at; callee find_close_colony; callee get_treaty; callee land_region_at; callee popm), the runner-up gets 5, and the arity agrees (0).
 */

#include "colonization.h"

#include "tutflag.h"
#include "jobs.h"
#include "units.h"
#include "terrain.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern int terrain_is_ocean(int x, int y);

extern int find_close_colony(int x, int y, int nation, int land);
extern int water_access(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int resource_at(int x, int y);
extern int land_region_at(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern unsigned char feature_get(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */

void check_tut(void)
{
    int u;
    int nation;
    int other;
    int terrain;
    int adj_terrain;
    int ok;
    int x;
    int y;
    int flag;
    int coast;
    int nx;
    int ny;
    int food_squares;
    int deco;
    int colony;
    int spec;
    int forests;
    int region;
    int col;
    register int i;
    register int best;

    u = g_game.selected_unit;
    if (g_units[u].orders != ORDERS_NONE)
        return;
    nation = NATION(g_units[u]);
    x = g_units[u].x;
    y = g_units[u].y;
    if (g_game.turn_counter == 0 && !(g_game.hint_flags & TUT_SHOWN_1)) {
        popup_say(0, g_unit_types[g_units[u].type].name);
        popm("TUTORIAL1", 0);
        g_game.hint_flags |= TUT_SHOWN_1;
        goto done;
    }
    if (!(g_game.hint_flags & TUT_SHOWN_11) && g_game.turn_counter < 0x14
        && g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST
        && g_units[u].cargo_count == 0 && g_units[u].link < 0 && g_units[u].link2 < 0) {
        popup_say(0, g_unit_types[g_units[u].type].name);
        popup_say(1, g_nation_name_word[nation]);
        popm("TUTORIAL11", 0);
        g_game.hint_flags |= TUT_SHOWN_11;
        goto done;
    }
    if (!(g_game.tutorial_flags & 1) && g_game.turn_counter < 0x14 && g_units[u].type == UNIT_PIONEERS
        && g_nation_colony_count[nation] == 0 && !terrain_is_ocean(x, y)) {
        popm("TUTORIAL13", 3);
        g_game.tutorial_flags |= 1;
        goto done;
    }
    if (!(g_game.tutorial_flags & 2) && g_game.turn_counter < 0x14 && !terrain_is_ocean(x, y)
        && g_units[u].type == UNIT_SOLDIERS) {
        popm("TUTORIAL14", 1);
        g_game.tutorial_flags |= 2;
        goto done;
    }
    if (g_game.turn_counter < 0x28 && !(g_game.tutorial_flags & 8) && g_units[u].type == UNIT_COLONISTS) {
        col = find_colony(x, y);
        if (col >= 0) {
            set_name_slot_1008_cdca(0, g_colonies[col].name);
            popm("TUTORIAL15", 5);
            g_game.tutorial_flags |= 8;
            goto done;
        }
    }
    if (!(g_game.hint_flags & TUT_SHOWN_3) && g_nation_colony_count[nation] == 0
        && g_units[u].type == UNIT_PIONEERS) {
        terrain = terrain_at(x, y);
        flag = 1;
        if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE
            || terrain == TERRAIN_MOUNTAINS || terrain == TERRAIN_HILLS)
            flag = 0;
        colony = find_close_colony(g_units[u].x, g_units[u].y, -1, -1);
        if (colony >= 0 && g_nearest_dist == 1)
            flag = 0;
        coast = water_access(g_units[u].x, g_units[u].y);
        if (coast != 0)
            coast = continent_at(g_ocean_x, g_ocean_y) == 1;
        if (coast == 0)
            flag = 0;
        if (flag != 0) {
            best = -1;
            food_squares = 0;
            for (i = 0; i < 9; i++) {
                nx = g_dir_dx[i] + x;
                ny = g_dir_dy[i] + y;
                terrain = terrain_at(nx, ny);
                if (g_terrain_rows[terrain].yield[0] != 0)
                    food_squares++;
                deco = resource_at(nx, ny);
                if (deco != -1) {
                    switch (deco) {
                    case 2:
                        if (best < 0)
                            best = 0;
                        break;
                    case 3:
                        best = 3;
                        break;
                    case 4:
                        best = 2;
                        break;
                    case 5:
                        best = 1;
                        break;
                    case 8:
                    case 9:
                        best = 4;
                        break;
                    case 10:
                        if (best < 0)
                            best = 5;
                        break;
                    case 12:
                        best = 7;
                        break;
                    case 6:
                    case 13:
                        best = 6;
                        break;
                    }
                }
            }
            region = land_region_at(x, y);
            if (region > 0 && g_region_size[region] < 7)
                food_squares++;
            if (best >= 0 && food_squares >= 5) {
                popup_say(0, g_goods_name_word[best]);
                popm("TUTORIAL3", 3);
                g_game.hint_flags |= TUT_SHOWN_3;
                goto done;
            }
        }
    }
    if (!(g_game.hint_flags & TUT_SHOWN_8) && g_units[u].type == UNIT_COLONISTS
        && ((int)g_units[u].spec == JOB_COLONIST_ALT || (int)g_units[u].spec == JOB_INDENTURED_SERVANT)) {
        flag = 0;
        for (i = 0; i < 8; i++) {
            other = i + 4;
            if (get_treaty(nation, other) & 0x20)
                flag = 1;
        }
        if (flag != 0) {
            spec = g_units[u].spec;
            if (spec == JOB_COLONIST_ALT)
                spec = 0x13;
            popup_say(0, g_spec_recs[spec].name);
            popm("TUTORIAL8", 5);
            g_game.hint_flags |= TUT_SHOWN_8;
            goto done;
        }
    }
    if (!(g_game.hint_flags & TUT_SHOWN_9) && g_units[u].type == UNIT_PIONEERS) {
        colony = find_close_colony(x, y, -1, -1);
        if (colony >= 0 && g_nearest_dist == 1 && g_cur_colony_rec->nation == (unsigned char)nation
            && !(feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD))) {
            terrain = terrain_at(x, y);
            if ((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)
                || terrain == TERRAIN_MOUNTAINS || terrain == TERRAIN_HILLS) {
                popm("TUTORIAL9", 3);
                g_game.hint_flags |= TUT_SHOWN_9;
                goto done;
            }
        }
    }
    if (!(g_game.hint_flags & TUT_SHOWN_10) && g_units[u].type == UNIT_PIONEERS) {
        colony = find_close_colony(x, y, -1, -1);
        if (colony >= 0 && g_nearest_dist == 1 && g_cur_colony_rec->nation == (unsigned char)nation
            && !(feature_get(x, y) & MAP1_PLOWED)) {
            terrain = terrain_at(x, y);
            ok = (terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC);
            forests = 0;
            if ((terrain & 7) < 2 || (terrain & 7) > 5)
                ok = 0;
            if (g_game.turn_counter < 0x10)
                ok = 0;
            for (i = 0; i < 8; i++) {
                nx = g_cur_colony_rec->x + g_dir_dx[i];
                ny = g_cur_colony_rec->y + g_dir_dy[i];
                adj_terrain = terrain_at(nx, ny);
                if (adj_terrain >= TERRAIN_PLAINS && adj_terrain <= TERRAIN_SAVANNAH)
                    ok = 0;
                if ((adj_terrain >= TERRAIN_FOREST_FIRST && adj_terrain < TERRAIN_FOREST_2_FIRST) || (adj_terrain >= TERRAIN_FOREST_2_FIRST && adj_terrain < TERRAIN_ARCTIC))
                    forests++;
            }
            if (forests < 2)
                ok = 0;
            if (ok != 0 || (terrain >= TERRAIN_PLAINS && terrain <= TERRAIN_SAVANNAH)) {
                popm("TUTORIAL10", 3);
                g_game.hint_flags |= TUT_SHOWN_10;
                goto done;
            }
        }
    }
done:
    ;
}
