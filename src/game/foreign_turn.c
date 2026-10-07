/* foreign_turn: candidate for 1028:4c8b (1694 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1694, the census figure; 1028:5329 starts immediately after, so there is no dead
 * second epilogue.
 *
 * A COMPUTER NATION'S TURN, run by `GameBrains_DoViceroy` when the nation's
 * `g_nation_text[n].control` is 1 (0 is a human player). It does the bookkeeping for the nation first and
 * then moves its units until none of them has anything left to do. `empty_1038_af7e(step, colour)` is called at six points along the way (steps 0..5,
 * with the colour `g_nation_colour[nation] - 8`) and does nothing in the shipped build.
 *
 * BOOKKEEPING, in order:
 *   - the random generator is reseeded from DGROUP:0x4b86, and the nation becomes current;
 *   - DGROUP:0x4c0a counts the regions (SEG20:0xdbf0, sixteen of them) with flag 8;
 *   - THE SHOPPING LIST at SEG24:0x65e, one byte per goods. Each colony of the nation adds one for
 *     the goods it asks for (+0x8d), another for muskets when that export IS muskets, and one each for muskets,
 *     horses and tools it has none of. Colonies with flag 0x10 at +0x1b are counted into
 *     `g_colonies_with_room[nation]`, and bit 5 of +0x1c is cleared in every one. The list is copied to SEG24:0x64e
 *     before the units take their share off: the cargo in every ship, and a set of tools for each
 *     pioneer. A wagon train whose home (+6) is one of the nation's own colonies sets that colony's bit 5
 *     back;
 *   - against each of the four Europeans, a pair flagged 8 whose timer at Nation+0x40 has run out
 *     has a one-in-four chance of dropping flags 0x48 and taking flag 1 -- then every timer ticks down;
 *   - `foreign_country`, `clear_plan` and `plan_strategy` run, with a music check between.
 *
 * THE UNITS are driven from the last to the first in two passes, repeating both until a whole round
 * passes without a unit acting. The first pass only moves wagon trains, treasure trains and artillery;
 * the second moves everything. A unit is run (`foreign_unit`) for as long as
 * `enemy_is_active` says it still has moves, and the same unit coming up more than twenty times in a
 * row (DGROUP:0x4c14/0x4c16) has its rating refreshed to break the loop. When one of the three special
 * units finishes where it is without having created or lost a unit, its square is noted for the nation
 * with `add_plan` as kind 2, at priority 3 for artillery, 2 for a treasure train, 1 otherwise.
 * With DGROUP:0x88 set and the nation visible, the map follows the moving unit.
 *
 * SHIPPED QUIRKS. Two `if`s have empty bodies -- one test of the debug byte DGROUP:0x8a and one of a
 * local that is set to 0 and never changed -- and a third repeats the whole "visible and still here"
 * test to do nothing; the debugging code they guarded was compiled out. A second local is set to 0 and
 * never read.
 *
 * WHAT THE BYTES SETTLED: the nation compares are bytes, `== (unsigned char)nation`, as in
 * `new_turn_bookkeeping`. `add_plan` takes ints in this module: the unit's
 * coordinates are zero-extended before they are pushed. The `switch` bodies are in the order
 * artillery, treasure, wagon/default, while the compare chain runs by value.
 *
 * RENAMED 2026-09-26 from ai_nation_turn: the Mac build's foreign_turn(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DoViceroy; callee add_plan; callee on_map), the runner-up gets 2, and the arity agrees (1). foreign_turn calls clear_plan, plan_strategy and add_plan, as this calls the rebuild, ai_survey_and_assign and add_plan.
 */

#include "colonization.h"

#include "aicol.h"
#include "units.h"
#include "goods.h"
#include "region.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern int g_units_expelled;                          /* DGROUP:0x1182, in this module */

extern Nation g_nations[];              /* SEG20:0xc2f4 */

typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;            /* SEG20:0x1566 */
extern GameState g_game;   /* SEG20:0x7782 */

extern void random_activate(int seed);
extern void set_europe(int n);
extern void bottom_box(int colour);

extern void set_colony(int colony);
extern int get_cargo(int u, int slot);
extern int get_treaty(int nation_a, int nation_b);

extern void check_music(void);
extern void end_unit_turn(int u);

#include "classes.h"
extern void GameWin_Redraw(GameWin *w);
extern int enemy_is_active(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void foreign_country(int nation);
extern void clear_plan(int nation);
extern void plan_strategy(int nation);
extern void foreign_unit(int u);
extern void add_plan(int nation, int x, int y, int kind, int priority);

void foreign_turn(int nation)
{
    int i;
    int acted;
    int unused;
    int cargo;
    int colour;
    int count;
    int pass;
    int special;
    int repeat;
    int dbg;
    register int u;
    register int priority;

    dbg = 0;
    g_ai_repeat_unit = -1;
    g_units_expelled = 0;
    random_activate(g_random_seed);
    g_game.turn_nation = nation;
    set_europe(nation);
    bottom_box(g_nation_colour[nation]);
    colour = g_nation_colour[nation] - 8;
    memset_rtl(g_goods_wanted, 0, 0x10);
    g_colonies_with_room[nation] = 0;
    g_garrisoned_regions = 0;
    for (i = 0; i < 16; i++)
        if (g_region_flags[i] & REGION_GARRISONED)
            g_garrisoned_regions++;
    empty_1038_af7e(0, colour);
    for (i = 0; i < g_game.colony_count; i++) {
        if (g_colonies[i].nation == (unsigned char)nation) {
            foreign_colony(i);
            if ((int)g_cur_colony_rec->wanted_goods >= 0)
                g_goods_wanted[(int)g_cur_colony_rec->wanted_goods]++;
            if ((int)g_cur_colony_rec->wanted_goods == GOODS_MUSKETS)
                g_goods_wanted[GOODS_MUSKETS]++;
            if (g_cur_colony_rec->stock[GOODS_MUSKETS] == 0)
                g_goods_wanted[GOODS_MUSKETS]++;
            if (g_cur_colony_rec->stock[GOODS_HORSES] == 0)
                g_goods_wanted[GOODS_HORSES]++;
            if (g_cur_colony_rec->stock[GOODS_TOOLS] == 0)
                g_goods_wanted[GOODS_TOOLS]++;
            if (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM)
                g_colonies_with_room[nation]++;
            g_cur_colony_rec->status &= ~0x20;
        }
    }
    empty_1038_af7e(1, colour);
    memcpy_rtl(g_goods_wanted_prev, g_goods_wanted, 0x10);
    unused = 0;
    for (u = 0; u < g_game.unit_count; u++) {
        if (NATION(g_units[u]) == (unsigned char)nation) {
            if (g_units[u].type == UNIT_WAGON_TRAIN && (int)g_units[u].home >= 0) {
                set_colony((int)g_units[u].home);
                if (g_cur_colony_rec->nation == (unsigned char)nation)
                    g_cur_colony_rec->status |= 0x20;
            }
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                for (i = 0; i < g_units[u].cargo_count; i++) {
                    cargo = get_cargo(u, i);
                    g_goods_wanted[cargo]--;
                }
            }
            if (g_units[u].type == UNIT_PIONEERS)
                g_goods_wanted[GOODS_TOOLS]--;
        }
    }
    for (i = 0; i < 4; i++) {
        if ((get_treaty(nation, i) & 8) && g_nations[nation].timer[i] <= 0
            && rand_range(0, 3) == 0) {
            g_nations[nation].rel[i] &= ~0x48;
            g_nations[nation].rel[i] |= 1;
        }
        if (g_nations[nation].timer[i] > 0)
            g_nations[nation].timer[i]--;
    }
    empty_1038_af7e(2, colour);
    check_music();
    foreign_country(nation);
    empty_1038_af7e(3, colour);
    check_music();
    clear_plan(nation);
    plan_strategy(nation);
    empty_1038_af7e(4, colour);
    check_music();
    do {
        check_music();
        if (g_debug_mode != 0)
            ;
        for (pass = 0; pass < 2; pass++) {
            acted = 0;
            for (u = g_game.unit_count - 1; acted == 0 && u >= 0; u--) {
                special = g_units[u].type == UNIT_WAGON_TRAIN
                       || g_units[u].type == UNIT_TREASURE_TRAIN
                       || g_units[u].type == UNIT_ARTILLERY;
                repeat = pass != 0 || special != 0;
                while (enemy_is_active(u) != 0 && repeat != 0) {
                    if (g_ai_repeat_unit == u) {
                        g_ai_repeat_count++;
                        if (g_ai_repeat_count > 0x14)
                            end_unit_turn(u);
                    } else {
                        g_ai_repeat_unit = u;
                        g_ai_repeat_count = 0;
                    }
                    if (g_autoplay != 0 && (g_game.view_nation == nation || g_game.reveal_all != 0)) {
                        g_game.selected_unit = u;
                        g_game.view_mode = 0;
                        if (on_map(g_units[u].x, g_units[u].y)) {
                            g_cursor_x = g_units[u].x;
                            g_cursor_y = g_units[u].y;
                        }
                        GameWin_Redraw((GameWin *)&g_stat_win);
                        count = g_game.unit_count;
                    }
                    if (dbg != 0)
                        ;
                    foreign_unit(u);
                    if (g_game.unit_count == count && special != 0 && enemy_is_active(u) == 0) {
                        switch (g_units[u].type) {
                        case UNIT_ARTILLERY:
                            priority = 3;
                            break;
                        case UNIT_TREASURE_TRAIN:
                            priority = 2;
                            break;
                        case UNIT_WAGON_TRAIN:
                        default:
                            priority = 1;
                            break;
                        }
                        add_plan(nation, g_units[u].x, g_units[u].y, 2, priority);
                    }
                    if (g_autoplay != 0 && (g_game.view_nation == nation || g_game.reveal_all != 0)
                        && g_game.unit_count == count && enemy_is_active(u) == 0)
                        ;
                    acted = 1;
                    if (g_debug_mode != 0)
                        g_game.view_mode = 1;
                }
            }
        }
    } while (acted != 0);
    empty_1038_af7e(5, colour);
}
