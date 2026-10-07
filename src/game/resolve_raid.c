/* resolve_raid: candidate for 1020:34b4 (2595 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 2595, not the census 2577. The trailing 18 bytes are this function's two jump tables,
 * which Borland puts in the code segment after the epilogue; 1020:3ed7 starts right after them.
 *
 * A NATIVE RAIDING PARTY REACHES A COLONY. `native` is the tribe's nation index (4 and up), `colony`
 * the colony, `settlement` the village the raid came from, and `force` makes the raid land whatever
 * the dice say. `1020:4460` is the only caller. It owns the six RAID messages.
 *
 * WHETHER IT LANDS: the colony's defence is 1 plus three per stockade level (building count 0). A roll
 * of 0..12 less one, plus `difficulty - 2` against a human colony, must reach it unless `force` is set.
 *
 * WHAT IT DOES, a d4:
 *   1. STORES -- a goods with at least 10 in the warehouse, found in up to 100 draws. A tribe with no
 *      horses goes for horses on its first draw half the time if the colony has more than 52. Muskets
 *      are skipped while `rand(200) - difficulty * draws` is still above the turn number.
 *   2. BURN -- a random building the colony has (up to 100 draws), never 0x23, 0x27, 0x15, 0x18, 0x1b,
 *      0, 1, 2 or 0x20, never one whose top level is 9, and when the colony is building an upgrade,
 *      never the same line. The draw climbs to the highest level present.
 *   3. SHIP -- the first ship in the colony's square, if there is one.
 *   4. GOLD -- `pop * treasury / (DGROUP:0x4b54[nation] + 1) + 10`, capped at 0x7fff, then drawn from
 *      50 up to that; it fails if the nation cannot pay or the draw is under 50.
 *   Early in a game on the two easiest levels (turn below `(2 - difficulty) * 40`), BURN and SHIP become
 *   nothing. BURN turns into STORES on a high roll or when the colony has a fort (flag 1), GOLD
 *   into STORES with flag 0, SHIP into nothing with a fortress (flag 2), and STORES into nothing with flag 0
 *   on a roll over the difficulty.
 *
 * THE OUTCOME: RAIDWREAK is shown to a computer colony's owner. A human hears the result:
 *   - STORES takes `rand(min(10, stock/2), stock/2)` (at least 1, at most the stock). Horses give the
 *     tribe a horse and 25 toward its herd, muskets one arm or two for 50 or more. RAIDSTORES.
 *   - BURN removes the building. The warehouse and the capitol (0xf and 0x1e) lose one of their counters at
 *     +0x95/+0x96 and only go when it reaches 0. Any other building with a negative byte at +3 also
 *     sends every colonist working its goods back to JOB_CARPENTER. RAIDBURN.
 *   - SHIP goes to `special_kill` as destroyed. RAIDSHIP.
 *   - GOLD leaves the treasury, and the amount is kept in SEG31:0x2d4 for the message. RAIDGOLD.
 *   - nothing: RAIDNOTHING (with sound 0x5b, which plays nothing).
 *   Every real outcome raises the tribe's alarm (4, 12, 16, 8) unless the pair has flag 2. And the
 *   raiding village's alarm toward the colony's nation is reset to 0.
 *
 * WHAT THE BYTES SETTLED: the gold arithmetic is 32-bit throughout, so both `pop` and the divisor are
 * widened to long before the operator -- `(long)pop * gold / ((long)g_nation_population[cn] + 1)`. The muskets
 * test is a `continue` in the `do` (a JCC to the condition followed by `EB 00`). The second switch
 * needs `case 0:` beside `default:` for its table to start at 0. `building` and `cn` are the two `register`
 * locals, SI and DI in declaration order (rule 53); `min_ptr_1020_b325` is the one NOP-padded 
 * call, and so the one extern (rule 47).
 *
 * TWO HORSE FIELDS, and this file is the only one that names both. Tribe+8 is a byte this
 * raid increments by ONE and Tribe+0xa is an int it adds 25 to -- "a horse and 25 toward
 * its herd", as the note above says. Four other files name only the int, and call it
 * `horses`; the byte is `f8` to them. The int is `horses` here too now, so the corpus
 * agrees on it, and the byte keeps the name this function earned for it.
 */

#include "colonization.h"

#include "difficul.h"
#include "control.h"
#include "building.h"
#include "units.h"
#include "goods.h"
#include "nations.h"
#include "sounds.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_tribe(int tribe);
extern void set_colony(int colony);
extern int get_name(int nation);
extern void popup_say(int slot, int word);
extern int building_level(int bit);
extern void random_activate(int seed);

extern int has_building(int bit);
extern int get_production_class(int item, int *idx);
extern int base_building(int building);
extern int top_unit_in_square(int x, int y);
extern int any_ship_in_stack(int u);
extern int next_unit(int u);

extern int popm(char *msg, int n);
extern void set_mood(int kind);
extern void force_music(int type);

extern void sound_play(int id);
extern int get_treaty(int nation_a, int nation_b);
extern void piss_off(int tribe, int nation, int alarm, int a);
extern void set_building(int bit, int on);
extern int job_of_building(int building);
extern int get_job(int colonist);
extern int set_job(int colonist, int job);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int special_kill(int u, int a, int b, int c, int d);

void resolve_raid(int native, int colony, int settlement, int force, int unused1)
{
    int tribe;
    int outcome;
    int defence;
    int roll;
    int goods;
    int amount;
    int j;
    int tries;
    int ok;
    int up;
    int unit;
    int jg;
    int bkind;
    long gold;
    int t1;
    int t2;
    register int building;
    register int cn;

    tribe = native - 4;
    set_tribe(tribe);
    set_colony(colony);
    cn = g_cur_colony_rec->nation;
    popup_say(0, get_name(native));
    set_name_slot_1020_a94a(1, g_cur_colony_rec->name);
    defence = 1;
    defence += building_level(BUILDING_STOCKADE) * 3;
    random_activate(g_random_seed);
    roll = rand_range(0, 0xc) - 1;
    if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN)
        roll += g_game.difficulty - 2;
    if (roll >= defence || force != 0) {
        outcome = rand_range(1, 4);
        if ((2 - g_game.difficulty) * 0x28 > g_game.turn_counter && g_game.difficulty <= DIFFICULTY_EXPLORER
            && (outcome == 2 || outcome == 3))
            outcome = 0;
        if (outcome == 2) {
            if (rand_range(0, 8) > (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN
                                    ? g_game.difficulty : 1) + 2)
                outcome = 1;
            if (has_building(BUILDING_FORT))
                outcome = 1;
        }
        if (outcome == 4 && has_building(BUILDING_STOCKADE))
            outcome = 1;
        if (outcome == 3 && has_building(BUILDING_FORTRESS))
            outcome = 0;
        if (outcome == 1 && has_building(BUILDING_STOCKADE) && rand_range(0, 8) > g_game.difficulty)
            outcome = 0;
        switch (outcome) {
        case 1:
            tries = 0;
            do {
                tries++;
                goods = rand_range(0, 0xf);
                if (g_cur_tribe_rec->horse_herds == 0 && tries == 1
                    && g_cur_colony_rec->stock[goods] > 0x34 && rand_range(0, 1) == 0)
                    goods = GOODS_HORSES;
                if (goods == GOODS_MUSKETS
                    && rand_range(0, 0xc8) - g_game.difficulty * tries > g_game.turn_counter)
                    continue;
            } while (tries < 100 && g_cur_colony_rec->stock[goods] < 0xa);
            if (tries >= 100)
                outcome = 0;
            break;
        case 2:
            tries = 0;
            bkind = get_production_class((int)g_cur_colony_rec->item, 0);
            do {
                ok = 1;
                tries++;
                building = rand_range(0, 0x29);
                if (building == 0x23)
                    ok = 0;
                if (base_building(building) == 9)
                    ok = 0;
                if (bkind == 1 && base_building(building)
                                  == base_building((int)g_cur_colony_rec->item))
                    ok = 0;
                if (building == 0x27 || building == 0x15 || building == 0x18 || building == 0x1b || building == 0 || building == 1
                    || building == 2 || building == 0x20)
                    ok = 0;
            } while (tries < 100 && (has_building(building) == 0 || ok == 0));
            if (tries >= 100 || ok == 0) {
                outcome = 0;
            } else {
                do {
                    ok = 1;
                    up = g_buildings[building].next_level;
                    if (up >= 0 && has_building(up)) {
                        building = up;
                        ok = 0;
                    }
                } while (ok == 0);
            }
            break;
        case 3:
            unit = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
            if (any_ship_in_stack(unit)) {
                while (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST)
                    unit = next_unit(unit);
            } else
                outcome = 0;
            break;
        case 4:
            gold = g_nations[cn].gold;
            gold = (long)g_cur_colony_rec->pop * gold / ((long)g_nation_population[cn] + 1) + 10;
            gold = min_long_1020_a997(gold, 0x7fffL);
            gold = rand_range(0x32, (int)gold);
            if (g_nations[cn].gold < gold || gold < 0x32)
                outcome = 0;
            break;
        }
    } else
        outcome = 0;
    popup_say(3, get_adjective(cn));
    if ((cn >= NATION_EUROPEAN_COUNT || g_nation_text[cn].control != CONTROL_HUMAN) && outcome != 0)
        popm("RAIDWREAK", 3);
    if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
        if (outcome == 0)
            set_mood(2);
        else
            force_music(0x32);
    }
    switch (outcome) {
    case 1:
        if (g_cur_colony_rec->stock[goods] == 0)
            break;
        popup_say(2, g_goods_name_word[goods]);
        amount = *min_ptr_1020_b325((t2 = 10, &t2), (t1 = g_cur_colony_rec->stock[goods] >> 1, &t1));
        amount = rand_range(amount, g_cur_colony_rec->stock[goods] >> 1);
        amount = min_int_1020_a8ad(amount, g_cur_colony_rec->stock[goods]);
        amount = max_int_1020_a887(1, amount);
        g_cur_colony_rec->stock[goods] -= amount;
        if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
            sound_play(SOUND_GUNWHOOP);
            popm("RAIDSTORES", 5);
        }
        if (goods == GOODS_HORSES) {
            g_cur_tribe_rec->horse_herds++;
            g_cur_tribe_rec->horses += 0x19;
        }
        if (goods == GOODS_MUSKETS) {
            g_cur_tribe_rec->muskets++;
            if (amount >= 0x32)
                g_cur_tribe_rec->muskets++;
        }
        if ((get_treaty(g_cur_tribe_nation, cn) & 2) == 0)
            piss_off(tribe, cn, -4, 0);
        break;
    case 2:
        popup_say(2, g_buildings[building].name);
        if (building == BUILDING_WAREHOUSE) {
            g_cur_colony_rec->warehouses--;
            if (g_cur_colony_rec->warehouses == 0)
                set_building(BUILDING_WAREHOUSE, 0);
            else
                popup_say(2, g_buildings[BUILDING_WAREHOUSE_EXPANSIONS].name);
        } else if (building == BUILDING_CAPITOL) {
            g_cur_colony_rec->capitols--;
            if (g_cur_colony_rec->capitols == 0)
                set_building(BUILDING_CAPITOL, 0);
            popup_say(2, g_buildings[BUILDING_CAPITOL_EXPANSION].name);
        } else {
            if ((int)g_buildings[building].prev_level < 0) {
                jg = job_of_building(building);
                if (jg >= 0) {
                    for (j = 0; j < g_cur_colony_rec->pop; j++)
                        if (get_job(j) == jg)
                            set_job(j, 0xd);
                }
            }
            set_building(building, 0);
        }
        if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
            sound_play(SOUND_FIRE1);
            popm("RAIDBURN", 5);
        }
        if ((get_treaty(g_cur_tribe_nation, cn) & 2) == 0)
            piss_off(tribe, cn, -0xc, 0);
        break;
    case 3:
        popup_say(2, g_unit_types[g_units[unit].type].name);
        if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
            sound_play(SOUND_BIGEXPLO);
            MSWaitTicks_1020_a9c5(0xf);
            sound_play(SOUND_BOARD);
            popm("RAIDSHIP", 5);
        }
        special_kill(unit, -1, 1, g_units[unit].x, g_units[unit].y);
        if ((get_treaty(g_cur_tribe_nation, cn) & 2) == 0)
            piss_off(tribe, cn, -0x10, 0);
        break;
    case 4:
        g_text_args[0] = gold;
        g_nations[cn].gold -= gold;
        if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
            sound_play(SOUND_WHOOP);
            popm("RAIDGOLD", 5);
        }
        if ((get_treaty(g_cur_tribe_nation, cn) & 2) == 0)
            piss_off(tribe, cn, -8, 0);
        break;
    case 0:
    default:
        if (cn < NATION_EUROPEAN_COUNT && g_nation_text[cn].control == CONTROL_HUMAN) {
            sound_play(0x5b);
            popm("RAIDNOTHING", 5);
        }
        break;
    }
    g_settlements[settlement].anger[cn] = 0;
}
