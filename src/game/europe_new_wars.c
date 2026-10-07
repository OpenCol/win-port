/* PORT: this file's own declarations of popk removed: the header declares it from the definition (W1 step 3). */
/* europe_new_wars: candidate for 1048:7897 (892 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: europe_new_wars
 *
 * The King's declaration of war on another European power. Only for a human player who does
 * not have founding father 0x13 (FATHER_BENJAMIN_FRANKLIN) with (difficulty + 2) * the TURN
 * COUNTER (SEG20:0x7790) of
 * at least 800: the other three powers are surveyed -- powers with pair flag 0x20, powers at
 * peace (0x40) and powers with 0x20 but WITHOUT peace (`(t & 0x60) == 0x20`) -- and the fleet tables compared;
 * with at least one peace, no 0x20-without-peace power at all (any one ends the function, so the fleet totals compared next are always zero) and a roll under
 * 80 - 20*peace + 40*war against the difficulty, a random power at peace is chosen, the
 * KINGNEWWAR message filled (difficulty word, the King's name, the enemy) with a gift of
 * money (difficulty*100 + 100, more when the enemy is stronger, capped at (5-difficulty)*500)
 * and soldiers (1 or more, capped at 6 - difficulty), the money paid, the soldiers created in
 * Europe as veterans (specialty 0x15), peace cleared and war (0x10) set on both sides, and the
 * war's start recorded at SEG20:0x77ca. The module's one-line wrappers are defined here
 * because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from king_declares_war: the Mac build's europe_new_wars(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller europe_new_turn; callee get_treaty; callee has_father; callee popup_say; callee treaty_on), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "fathers.h"
#include "nations.h"
#include "jobs.h"
#include "units.h"
#include "records.h"

extern Nation g_nations[];   /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int has_father(int nation, int bit);
extern int get_treaty(int nation_a, int nation_b);
extern void random_activate(int seed);

extern void popup_say(int slot, int word);


extern int treaty_off(int nation, int other, int mask);
extern int treaty_on(int nation, int other, int mask);

extern void set_name_slot_1048_de36(int i, char *s);

extern void set_text_arg_i_1048_db6a(int slot, int v);

void europe_new_wars(void)
{
    int peace;
    int allies;
    int plain;
    int chance;
    int roll;
    int money;
    int units;
    int u;
    int i;
    int k;
    int total_n;
    int total_p;
    int n;
    int cur_nation;

    peace = 0;
    allies = 0;
    plain = 0;
    total_n = 0;
    total_p = 0;
    cur_nation = g_cur_nation;
    if (cur_nation >= 4 || g_nation_text[cur_nation].control != 0)
        goto done;
    if (has_father(cur_nation, FATHER_BENJAMIN_FRANKLIN) != 0)
        goto done;
    if ((g_game.difficulty + 2) * g_game.turn_counter < 800)
        goto done;
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        if (n == cur_nation)
            continue;
        if (g_game.mother_country_idx == n)
            continue;
        if (g_nations[n].flags & 4)
            continue;
        if (get_treaty(cur_nation, n) & 0x20)
            allies++;
        if (get_treaty(cur_nation, n) & 0x40)
            peace++;
        if ((get_treaty(cur_nation, n) & 0x60) == 0x20) {
            plain++;
            for (k = 1; k < 15; k++) {
                if (g_region_colony_count[cur_nation] != 0 && g_region_colony_count[n] != 0) {
                    total_n = g_military_power[n] + total_n;
                    total_p = g_military_power[cur_nation] + total_p;
                }
            }
        }
    }
    if (peace == 0 || plain != 0)
        goto done;
    if (total_n > total_p)
        goto done;
    random_activate(g_random_seed);
    chance = 80 - peace * 20 + plain * 40;
    roll = rand_range(0, chance);
    if (g_game.difficulty < roll)
        goto done;
    do {
        n = rand_range(0, 3);
    } while (n == cur_nation || (get_treaty(cur_nation, n) & 0x40) == 0 || (g_nations[n].flags & 4));
    units = 1;
    money = g_game.difficulty * 100 + 100;
    if (g_nation_strength[n] > g_nation_strength[cur_nation]) {
        units += (g_nation_strength[n] - g_nation_strength[cur_nation]) >> 3;
        money += (g_nation_strength[n] - g_nation_strength[cur_nation]) * 25;
    }
    units = min_int_1048_db44(units, 6 - g_game.difficulty);
    money = min_int_1048_db44(money, (5 - g_game.difficulty) * 500);
    popup_say(0, g_difficulty_words[g_game.difficulty]);
    set_name_slot_1048_de36(1, g_nation_text[cur_nation].leader);
    popup_say(2, get_adjective(n));
    set_text_arg_i_1048_db6a(0, money);
    set_text_arg_i_1048_db6a(1, units);
    popk("KINGNEWWAR");
    g_cur_nation_rec->gold += money;
    for (i = 0; i < units; i++) {
        u = create_unit(UNIT_SOLDIERS, cur_nation, cur_nation - 20, cur_nation - 20);
        if (u < 0)
            break;
        g_units[u].spec = JOB_SOLDIER;
    }
    treaty_off(cur_nation, n, 0x40);
    treaty_on(cur_nation, n, 0x10);
    g_game.war_started[n] = g_game.turn_counter;
done:
    return;
}
