/* perform_orders_clear: candidate for 1028:a324 (1220 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A PIONEER'S TURN OF CLEARING OR PLOWING the square it stands on. On forest (terrain 8..23) it
 * clears; elsewhere it plows, unless the square is already plowed or is water, and then the orders
 * are simply cleared.
 *
 * THE WORK takes the terrain's work turns (+4 of the 16-byte terrain record at SEG20:0x78f8) + 2
 * turns, halved for a HARDY PIONEER (JOB_PIONEER), counted in the unit's +0x16. When it is done the
 * counter and orders are cleared and:
 *   - CLEARING: a colony of the nation within 3 squares gets lumber. The amount is the terrain's
 *     lumber yield (+0xc), +1 when the colony square has a road or settlement, and only 1 without
 *     colony flag 0x24. It is x20, doubled for a hardy pioneer, capped by warehouse room, and a
 *     human hears CLEARCUT. The square's terrain drops by 8, from the forest ids to the open ids;
 *   - PLOWING: plane-1 MAP1_PLOWED is set.
 * The square is scrolled to and redrawn when the nation on turn is the viewer, or when the map is
 * revealed and the matching watch bit of 0x7784 is set.
 *
 * CLEARING NATIVE LAND. When the cleared square lies within the nearest village's reach (its tribe's
 * level) and neither has been paid for (MAP1_CLAIM_SETTLED) nor is European-owned, and
 * `buy_this` does not settle it, the tribe's alarm rises by `difficulty + 5` for a human (5
 * otherwise). It rises twice that within 2 squares, and three times that when adjacent.
 *
 * WHAT THE BYTES SETTLED: the work test and everything after it are nested `if`s (386 long
 * conditional jumps, rule 12), while the two early exits to `stop` are `goto`s. The lumber colony is
 * looked up with `NATION(g_units[u])` written out again. The human test for the alarm reads the nation on
 * turn (0x7796), not the unit's own nation.
 *
 * RENAMED 2026-09-26 from clear_forest: the Mac build's perform_orders_clear(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_plan; caller foreign_unit; callee find_close_colony; callee has_building; callee land_region_at), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "building.h"
#include "terrain.h"
#include "units.h"
#include "jobs.h"
#include "goods.h"
#include "nations.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int land_region_at(int x, int y);
extern unsigned char *map_loc(int x, int y);
extern unsigned char *feature_loc(int x, int y);

extern unsigned char feature_get(int x, int y);
extern void end_unit_turn(int u);
extern int find_close_colony(int x, int y, int nation, int land);
extern int has_building(int bit);
extern int get_warehouse(void);
extern int popm(char *msg, int n);
extern int possibly_center(int x0, int y0, int x1, int y1, int flag);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern int find_close_village(int x, int y, int nation, int region);
extern int is_colony(int x, int y);
extern int tribe_radius(int tribe);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void use_tools(int u);
extern int buy_this(int village, int nation, int x, int y);

void perform_orders_clear(int u)
{
    unsigned char *p0;
    unsigned char *p1;
    int terrain;
    int col;
    int lumber;
    int nation;
    int village;
    int alarm;
    int base;
    int level;
    int turns;
    int expert;
    int plow;
    int region;
    int cap;
    int amount;
    register int x;
    register int y;

    x = g_units[u].x;
    y = g_units[u].y;
    region = land_region_at(x, y);
    p0 = map_loc(x, y);
    p1 = feature_loc(x, y);
    terrain = terrain_at(x, y);
    nation = NATION(g_units[u]);
    if ((terrain >= TERRAIN_BOREAL_FOREST && terrain < TERRAIN_BOREAL_FOREST_2)
        || (terrain >= TERRAIN_BOREAL_FOREST_2 && terrain < TERRAIN_ARCTIC)) {
        plow = 0;
    } else {
        if (feature_get(x, y) & MAP1_PLOWED)
            goto stop;
        if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE)
            goto stop;
        plow = 1;
    }
    end_unit_turn(u);
    g_units[u].counter++;
    turns = g_terrain_rows[terrain].work_turns + 2;
    expert = (int)g_units[u].spec == JOB_PIONEER ? 1 : 0;
    if (expert)
        turns = turns >> 1;
    if (g_units[u].counter >= (unsigned char)turns) {
        g_units[u].counter = 0;
        g_units[u].orders = ORDERS_NONE;
        if (g_game.colony_count != 0 && plow == 0) {
            col = find_close_colony(x, y, NATION(g_units[u]), -1);
            if (col >= 0 && g_nearest_dist <= 3) {
                lumber = g_terrain_rows[terrain].lumber;
                if (feature_get(g_cur_colony_rec->x, g_cur_colony_rec->y) & (MAP1_SETTLEMENT | MAP1_ROAD))
                    lumber++;
                if (has_building(BUILDING_LUMBER_MILL) == 0)
                    lumber = 1;
                amount = lumber * 20 << expert;
                cap = get_warehouse();
                amount = min_int_1028_e376(amount, cap - g_cur_colony_rec->stock[GOODS_LUMBER]);
                amount = max_int_1028_e350(0, amount);
                if (amount != 0 && nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                    set_text_arg_i_1028_e3ee(0, amount);
                    set_name_slot_1028_e412(0, g_cur_colony_rec->name);
                    popm("CLEARCUT", 5);
                }
                g_cur_colony_rec->stock[GOODS_LUMBER] += amount;
            }
        }
        if (g_game.turn_nation == g_game.view_nation
            || (g_game.reveal_all != 0 && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000))))
            possibly_center(x, y, x, y, 1);
        if (plow == 0) {
            if (terrain >= TERRAIN_BOREAL_FOREST_2 && terrain < TERRAIN_ARCTIC)
                *p0 = *p0 - 8;
            else
                *p0 = *p0 - 8;
        } else
            *p1 |= MAP1_PLOWED;
        use_tools(u);
        if (g_game.turn_nation == g_game.view_nation
            || (g_game.reveal_all != 0 && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000))))
            DrawMapRegion(x - 1, y - 1, 3, 3, 1);
        if (plow == 0) {
            village = find_close_village(x, y, -1, region);
            if (village >= 0) {
                if (!(feature_get(x, y) & MAP1_CLAIM_SETTLED)) {
                    if (is_colony(x, y) < 0) {
                        level = tribe_radius(g_cur_tribe);
                        if (g_nearest_dist <= level) {
                            if (buy_this(village, nation, x, y) == 0) {
                                base = (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN ? g_game.difficulty : 0) + 5;
                                alarm = base;
                                if (g_nearest_dist <= 2)
                                    alarm += base;
                                if (g_nearest_dist <= 1)
                                    alarm += base;
                                piss_off(g_cur_tribe, nation, alarm, 2);
                            }
                        }
                    }
                }
            }
        }
    }
    goto end;
stop:
    g_units[u].orders = ORDERS_NONE;
end:
    ;
}
