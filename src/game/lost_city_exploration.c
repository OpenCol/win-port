/* lost_city_exploration: candidate for 1038:7c74 (2535 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2535, not the census 2517. The trailing 18 bytes are the nine-entry jump table of
 * the closing `switch`, which Borland puts in the code segment after the epilogue.
 *
 * A UNIT ENTERS A LOST CITY RUMOUR at (x, y). `1020:cfb8` calls it. It picks one of the nine LOSTCITY
 * outcomes, tells a human player, and applies it. (The curated name, lost_city_exploration, was
 * taken from SCREWED alone; that is only one branch of outcome 4.)
 *
 *   1 Fountain of Youth      -- eight free recruits in Europe (`europe_recruit`)
 *   2 Seven Cities of Cibola -- a treasure train worth 20 + 1..20 + 10 per scout level
 *   3 ruins                  -- 3d8 x 10 gold, times (scout level + 2) / 2 for a scout
 *   4 burial mounds          -- BURIAL1 nothing, BURIAL2 3d8 x 10 gold, BURIAL3 a treasure train;
 *                               a village nearby may take offence: SCREWED and alarm 100
 *   5 vanished               -- the unit is lost
 *   6 nothing but rumours
 *   7 friendly tribe's gift  -- 4d10 x 2 gold
 *   8 holy shrines           -- the nearest village within two squares gains 5 + 1..6 +
 *                               5 * (difficulty - scout level) alarm
 *   9 colony survivors       -- a free colonist (unit type 0) joins
 *
 * THE DRAW is `max(1..9, rerolls)`. SCOUT LEVEL is 1 for a scout, 2 for an expert scout (specialty
 * 0x16), and one more with Hernando de Soto (father flag 7), who also rerolls vanish, shrines and
 * nothing -- each reroll raises the floor, to at most 3, so Fountain and Cibola drop out after two.
 * Vanish and shrines are both re-rolled into nothing unless a `1..scout+1` roll comes up 1. The
 * Fountain needs `(t & 7) >= 4` terrain (grassland, savannah, marsh, swamp and the forests on them) and
 * four rumours already explored (DGROUP:0x1942), or de Soto; else it is vanish (10%) or nothing; it is always Cibola with SEG20:0x7784 bit 0. Cibola needs
 * desert, scrub forest, mountains or hills (`(t & 7) == 1`, or 27/28) -- or a coin flip under de Soto --
 * and fewer than
 * seven treasures so (DGROUP:0x1943), or else it is vanish, shrines or nothing by the d100. A
 * nation's FIRST vanish is turned into burial mounds (bit 6 of its +0 byte at SEG20:0x7840), a nation
 * with few colonies cannot vanish at all, and a pioneer vanishes only half as often.
 *
 * SHIPPED QUIRKS. `tribe` is only assigned on the shrines path and in outcome 4, yet the alarm call
 * after the message reads it; that call is guarded by `alarm != 0`, which only the shrines path sets.
 * `case 2: break;` is a real (empty) arm of the final switch.
 *
 * WHAT THE BYTES SETTLED: the first-vanish flag test is `!(f0 & 0x40)` on an UNSIGNED byte -- only
 * the `!` spelling loads and zero-extends the byte before the test, and only `unsigned char` gives
 * `MOV AH,0`. `set_europe` takes an int in this module (the nation is zero-extended before
 * the push). `outcome` and `u` are the two `register` locals, SI and DI in declaration order
 * (rule 53); `min_ptr_1038_8dc4` is the one NOP-padded call and so the one extern (rule 47).
 *
 * RENAMED 2026-09-26 from desecrate_burial_grounds: the Mac build's lost_city_exploration(short, short, short): the whole nine-outcome Lost City Rumour resolver, bound by call-graph agreement (21 of its Mac call edges already bound land here, the runner-up 4), alignment in move_unit, and size (1.06x); the arity is (unit, x, y) on both sides. The record said desecrate_burial_grounds -- one branch's SCREWED text -- while the source already defined it as lost_city_rumour; both now carry the developers' name.
 *
 * RENAMED 2026-09-26 from lost_city_rumour: the Mac build's lost_city_exploration(short, short, short): the whole nine-outcome Lost City Rumour resolver, bound by call-graph agreement (21 of its Mac call edges already bound land here, the runner-up 4), alignment in move_unit, and size (1.06x); the arity is (unit, x, y) on both sides. The record said desecrate_burial_grounds -- one branch's SCREWED text -- while the source already defined it as lost_city_rumour; both now carry the developers' name.
 *
 * A LIKELY SHIPPED SLIP in outcome 8 (holy shrines): `village <= 2` tests the INDEX that
 * find_close_village returns, not the distance it leaves in g_nearest_dist. It is true when no
 * village is found (-1, with g_cur_tribe left from an earlier call) and for villages 0..2 only;
 * `g_nearest_dist <= 2` reads like what was meant. The bytes fix the index test. (The local was
 * named `dist` until 2026-09-27.)
 */

#include "colonization.h"

#include "fathers.h"
#include "control.h"
#include "woodcut.h"
#include "gameflag.h"
#include "terrain.h"
#include "sounds.h"
#include "jobs.h"
#include "units.h"
#include "nations.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int has_father(int nation, int bit);
extern void random_activate(int seed);

extern void free_galleon(int n);

extern void popup_say(int slot, int word);
extern int get_treaty(int nation_a, int nation_b);

extern void sound_play(int id);

extern void set_mood(int kind);
extern void force_mood(int mood);
extern int popm(char *msg, int n);
extern void force_music(int type);
extern void set_europe(int n);

extern void DrawMapRegion(int x, int y, int a, int b, int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int find_close_village(int x, int y, int nation, int owner);

extern int piss_off(int tribe, int nation, int alarm, int a);
extern void delete_unit(int u);
extern void move_to_top(int u);

void lost_city_exploration(int unit, int x, int y)
{
    int human;
    int redraw;
    int roll;
    int terrain;
    int flag;
    int newu;
    int answer;
    int kind;
    int tribe;
    int village;
    int alarm;
    int scout;
    int gold;
    int i;
    int desoto;
    int treasure;
    int mapchange;
    int rerolls;
    int t1;
    int t2;
    char buf[20];
    register int outcome;
    register int u;

    u = unit;
    redraw = 0;
    answer = 1;
    alarm = 0;
    gold = 0;
    desoto = 0;
    treasure = 0;
    mapchange = 0;
    rerolls = 0;
    human = g_game.turn_nation == g_game.view_nation && g_game.turn_nation < NATION_EUROPEAN_COUNT
            && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN;
    scout = g_units[u].type == UNIT_SCOUTS;
    if (scout != 0 && (int)g_units[u].spec == JOB_SCOUT)
        scout++;
    if (has_father(g_game.turn_nation, FATHER_HERNANDO_DE_SOTO) && scout != 0) {
        desoto = 1;
        scout++;
    }
    g_rumours_seen++;
    random_activate(g_random_seed);
again:
    rerolls = *min_ptr_1038_8dc4((t2 = 3, &t2), (t1 = rerolls + 1, &t1));
    outcome = rand_range(1, 9);
    outcome = max_int_1038_86a6(outcome, rerolls);
    roll = rand_range(1, 100) + scout * 10;
    terrain = terrain_at(x, y);
    if (outcome == 5 && rand_range(1, scout + 1) != 1) {
        if (desoto != 0)
            goto again;
        outcome = 6;
    }
    if (outcome == 8 && rand_range(1, scout + 1) != 1) {
        if (desoto != 0)
            goto again;
        outcome = 6;
    }
    if (outcome == 1) {
        flag = terrain < TERRAIN_ARCTIC && (terrain & 7) >= 4;
        if ((flag == 0 || g_rumours_seen < 4) && desoto == 0) {
            if (roll <= 10)
                outcome = 5;
            else
                outcome = 6;
        }
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
            outcome = 2;
    }
    if (outcome == 2) {
        flag = terrain == TERRAIN_MOUNTAINS || terrain == TERRAIN_HILLS || (terrain < TERRAIN_ARCTIC && (terrain & 7) == 1);
        if (desoto != 0 && rand_range(0, 2) == 0)
            flag = 1;
        if (flag == 0 || g_rumours_seen < 1 || g_treasures_found >= 7) {
            if (roll <= 10)
                outcome = 5;
            else if (roll < 0x19)
                outcome = 8;
            else
                outcome = 6;
        } else {
            treasure = rand_range(1, 20) + scout * 10 + 20;
            u = create_unit(UNIT_TREASURE_TRAIN, NATION(g_units[u]), g_units[u].x, g_units[u].y);
            if (u < 0)
                goto done;
            g_units[u].spec = treasure;
            mapchange = 1;
            redraw = 1;
            g_treasures_found++;
            free_galleon(NATION(g_units[u]));
        }
    }
    if (outcome == 8) {
        if (desoto != 0)
            goto again;
        village = find_close_village(g_units[u].x, g_units[u].y, -1, -1);
        if (village <= 2) {
            tribe = g_cur_tribe;
            alarm = rand_range(1, 6) + (g_game.difficulty - scout) * 5 + 5;
            popup_say(0, get_adjective(g_cur_tribe_nation));
            if ((get_treaty(g_game.turn_nation, g_cur_tribe_nation) & 0x20) == 0) {
                tribe = -1;
                outcome = 6;
            }
        } else
            outcome = 6;
    }
    if (outcome == 3) {
        gold = (rand_range(1, 8) + rand_range(1, 8) + rand_range(1, 8)) * 10;
        if (scout != 0)
            gold = (scout + 2) * gold >> 1;
    }
    if (outcome == 7)
        gold = (rand_range(1, 10) + rand_range(1, 10) + rand_range(1, 10) + rand_range(1, 10)) * 2;
    if (outcome == 9) {
        g_name_slots[0][0] = 0;
        say_string(g_name_slots[0], g_nation_name[g_game.turn_nation]);
    }
    if (outcome == 5) {
        if (g_nation_population[g_game.turn_nation] <= 4 && g_nation_colony_count[g_game.turn_nation] <= 2)
            outcome = 6;
        if (g_units[u].type == UNIT_PIONEERS && g_nation_population[g_game.turn_nation] <= 8 && rand_range(1, 2) == 1)
            outcome = 6;
        if (!(g_nation_text[g_game.turn_nation].once_flags & 0x40)) {
            g_nation_text[g_game.turn_nation].once_flags |= 0x40;
            outcome = 4;
        }
    }
    if (outcome == 5 && desoto != 0)
        outcome = 6;
    if (outcome == 6 && desoto != 0)
        goto again;
    set_text_arg_i_1038_86cc(0, gold);
    set_text_arg_i_1038_86cc(1, treasure * 100);
    STRCPY(buf, "LOSTCITY");
    say_number(buf, outcome);
    if (human) {
        if (outcome == 1) {
            sound_play(0x37);
            woodcut(WOODCUT_THE_FOUNTAIN_OF_YOUTH);
        } else if (outcome == 2) {
            sound_play(SOUND_GOLDCITY);
        } else if (outcome == 4) {
            sound_play(SOUND_MUSIC_NATIVES);
        } else if (gold != 0) {
            set_mood(2);
            if (g_sound_music == 0)
                force_mood(2);
        }
    }
    if (human) {
        g_popup_dude = 3;
        answer = popm(buf, 3);
    }
    if (alarm != 0 && tribe >= 0)
        piss_off(tribe, NATION(g_units[u]), alarm, 0);
    if (outcome == 4 && answer == 1) {
        tribe = -1;
        village = find_close_village(x, y, -1, -1);
        if (village >= 0 && rand_range(1, (g_nearest_dist + 5) << scout) <= 3)
            tribe = g_cur_settlement_rec->owner - 4;
        if ((get_treaty(g_game.turn_nation, g_cur_tribe_nation) & 0x20) == 0)
            tribe = -1;
        if (roll < 0x19) {
            kind = 1;
            gold = 0;
        } else if (roll < 0x32 || (tribe < 0 && roll < 0x41)) {
            kind = 2;
            gold = (rand_range(1, 8) + rand_range(1, 8) + rand_range(1, 8)) * 10;
        } else {
            kind = 3;
            treasure = (rand_range(1, 8) + scout * 2) * 2 + 20;
            u = create_unit(UNIT_TREASURE_TRAIN, NATION(g_units[u]), g_units[u].x, g_units[u].y);
            if (u < 0)
                goto done;
            g_units[u].spec = treasure;
            if (human && tribe < 0)
                force_music(0x24);
            free_galleon(NATION(g_units[u]));
            mapchange = 1;
            redraw = 1;
        }
        set_text_arg_i_1038_86cc(0, gold);
        set_text_arg_i_1038_86cc(1, treasure * 100);
        STRCPY(buf, "BURIAL");
        say_number(buf, kind);
        if (human)
            popm(buf, 3);
        if (tribe >= 0) {
            if (human)
                force_music(0x32);
            popup_say(0, get_adjective(tribe + 4));
            if (human)
                popm("SCREWED", 3);
            piss_off(tribe, g_game.turn_nation, 100, 0);
        }
    }
    switch (outcome) {
    case 1:
        set_europe(NATION(g_units[u]));
        for (i = 0; i < 8; i++)
            europe_recruit(1, 0);
        break;
    case 2:
        break;
    case 5:
        if (human) {
            set_mood(1);
            if (g_sound_music == 0)
                force_mood(1);
        }
        delete_unit(u);
        redraw = 1;
        break;
    case 9:
        newu = create_unit(UNIT_COLONISTS, NATION(g_units[u]), x, y);
        if (newu < 0)
            goto done;
        move_to_top(u);
        break;
    }
    if (gold != 0)
        g_nations[g_game.turn_nation].gold += gold;
done:
    if (human && redraw) {
        DrawMapRegion(x - 3, y - 3, 7, 7, !mapchange);
        if (mapchange != 0)
            empty_1040_a184(8);
    }
}
