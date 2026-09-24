/* lost_city_rumour -- shipped at 1038:7c74.
 *
 * Ported by tools/port.py from matched/game/lost_city_rumour.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char g_rumours_seen;                 DGROUP:0x1942
 *   extern unsigned char g_treasures_found;              DGROUP:0x1943
 *   extern int far g_sound_music;                        DGROUP:0x00ba
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern unsigned char far g_nation_colony_count[4];   DGROUP:0x4b48
 *   extern unsigned char far g_nation_population[];      DGROUP:0x4b54
 *   extern int far g_random_seed;                        DGROUP:0x4b86
 *   extern int far g_nearest_dist;                       DGROUP:0x4bc6
 *   extern Settlement far * far g_cur_settlement_rec;    DGROUP:0x4c2c
 *   extern int far g_cur_tribe;                          DGROUP:0x4c30
 *   extern int far g_cur_tribe_nation;                   DGROUP:0x4c32
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern char far g_edit_bufs[][0x50];                 SEG31:0x00a4
 *
 * The file's own Unit, NationRec52, Nation, Settlement dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 create_unit_1038_2246(col_i16 kind, col_i16 nation, col_i16 x, col_i16 y);
col_i16 find_nearest_village(col_i16 x, col_i16 y, col_i16 a, col_i16 owner);
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 tribe_hostile(col_i16 v, col_i16 nation, col_i16 alarm, col_i16 a);
void set_text_arg_i_1038_86cc(col_i16 a, col_i16 v);
void unit_record_free(col_i16 u);
void unit_replace_at_self(col_i16 u);

#include "fathers.h"
#include "control.h"
#include "woodcut.h"
#include "gameflag.h"
#include "terrain.h"
#include "sounds.h"
#include "jobs.h"
#include "units.h"
#include "nations.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern col_i16 nation_flag_test(col_i16 n, col_i16 bit);
extern void seed_random(col_i16 a);
extern col_i16 * min_ptr_1038_8dc4(col_i16 *a, col_i16 *b);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void empty_1020_41ff(col_i16 n);
extern col_i16 lookup_with_two_overrides(col_i16 i);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 nation_pair_flags_get(col_i16 a, col_i16 b);
extern void say_string(char *dst, col_i16 word);

extern void say_number(char *buf, col_i16 n);
extern void play_sound_effect(col_i16 id);
extern void woodcut_show(col_i16 a);
extern void music_mood_store_1040_8936(col_i16 kind);
extern void music_set_mood(col_i16 a);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void music_request_track(col_i16 type);
extern void set_current_nation(col_i16 n);
extern void europe_recruit(col_i16 a, col_i16 b);
extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);
extern void empty_1040_a184(col_i16 a);

/* same object: 4-byte `push cs; call ` (rule 47) */








void lost_city_rumour(col_i16 unit, col_i16 x, col_i16 y)
{
    col_i16 human;
    col_i16 redraw;
    col_i16 roll;
    col_i16 terrain;
    col_i16 flag;
    col_i16 newu;
    col_i16 answer;
    col_i16 kind;
    col_i16 tribe;
    col_i16 dist;
    col_i16 alarm;
    col_i16 scout;
    col_i16 gold;
    col_i16 i;
    col_i16 desoto;
    col_i16 treasure;
    col_i16 mapchange;
    col_i16 rerolls;
    col_i16 t1;
    col_i16 t2;
    char buf[20];
    register col_i16 outcome;
    register col_i16 u;

    u = unit;
    redraw = 0;
    answer = 1;
    alarm = 0;
    gold = 0;
    desoto = 0;
    treasure = 0;
    mapchange = 0;
    rerolls = 0;
    human = g_turn_nation == g_view_nation && g_turn_nation < NATION_EUROPEAN_COUNT
            && g_nation52[g_turn_nation].control == CONTROL_HUMAN;
    scout = g_units[u].type == UNIT_SCOUTS;
    if (scout != 0 && (col_i16)g_units[u].spec == JOB_SCOUT)
        scout++;
    if (nation_flag_test(g_turn_nation, FATHER_HERNANDO_DE_SOTO) && scout != 0) {
        desoto = 1;
        scout++;
    }
    g_rumours_seen++;
    seed_random(g_random_seed);
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
        if (g_game_flags & GAMEFLAG_INDEPENDENCE)
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
            u = create_unit_1038_2246(UNIT_TREASURE_TRAIN, NATION(g_units[u]), g_units[u].x, g_units[u].y);
            if (u < 0)
                goto done;
            g_units[u].spec = treasure;
            mapchange = 1;
            redraw = 1;
            g_treasures_found++;
            empty_1020_41ff(NATION(g_units[u]));
        }
    }
    if (outcome == 8) {
        if (desoto != 0)
            goto again;
        dist = find_nearest_village(g_units[u].x, g_units[u].y, -1, -1);
        if (dist <= 2) {
            tribe = g_cur_tribe;
            alarm = rand_range(1, 6) + (g_difficulty - scout) * 5 + 5;
            set_name_slot_from_text(0, lookup_with_two_overrides(g_cur_tribe_nation));
            if ((nation_pair_flags_get(g_turn_nation, g_cur_tribe_nation) & 0x20) == 0) {
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
        g_edit_bufs[0][0] = 0;
        say_string(g_edit_bufs[0], g_nation_name[g_turn_nation]);
    }
    if (outcome == 5) {
        if (g_nation_population[g_turn_nation] <= 4 && g_nation_colony_count[g_turn_nation] <= 2)
            outcome = 6;
        if (g_units[u].type == UNIT_PIONEERS && g_nation_population[g_turn_nation] <= 8 && rand_range(1, 2) == 1)
            outcome = 6;
        if (!(g_nation52[g_turn_nation].f0 & 0x40)) {
            g_nation52[g_turn_nation].f0 |= 0x40;
            outcome = 4;
        }
    }
    if (outcome == 5 && desoto != 0)
        outcome = 6;
    if (outcome == 6 && desoto != 0)
        goto again;
    set_text_arg_i_1038_86cc(0, gold);
    set_text_arg_i_1038_86cc(1, treasure * 100);
    strcpy(buf, "LOSTCITY");
    say_number(buf, outcome);
    if (human) {
        if (outcome == 1) {
            play_sound_effect(0x37);
            woodcut_show(WOODCUT_THE_FOUNTAIN_OF_YOUTH);
        } else if (outcome == 2) {
            play_sound_effect(SOUND_GOLDCITY);
        } else if (outcome == 4) {
            play_sound_effect(SOUND_MUSIC_NATIVES);
        } else if (gold != 0) {
            music_mood_store_1040_8936(2);
            if (g_sound_music == 0)
                music_set_mood(2);
        }
    }
    if (human) {
        g_text_value = 3;
        answer = text_message_show(buf, 3);
    }
    if (alarm != 0 && tribe >= 0)
        tribe_hostile(tribe, NATION(g_units[u]), alarm, 0);
    if (outcome == 4 && answer == 1) {
        tribe = -1;
        dist = find_nearest_village(x, y, -1, -1);
        if (dist >= 0 && rand_range(1, (g_nearest_dist + 5) << scout) <= 3)
            tribe = g_cur_settlement_rec->owner - 4;
        if ((nation_pair_flags_get(g_turn_nation, g_cur_tribe_nation) & 0x20) == 0)
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
            u = create_unit_1038_2246(UNIT_TREASURE_TRAIN, NATION(g_units[u]), g_units[u].x, g_units[u].y);
            if (u < 0)
                goto done;
            g_units[u].spec = treasure;
            if (human && tribe < 0)
                music_request_track(0x24);
            empty_1020_41ff(NATION(g_units[u]));
            mapchange = 1;
            redraw = 1;
        }
        set_text_arg_i_1038_86cc(0, gold);
        set_text_arg_i_1038_86cc(1, treasure * 100);
        strcpy(buf, "BURIAL");
        say_number(buf, kind);
        if (human)
            text_message_show(buf, 3);
        if (tribe >= 0) {
            if (human)
                music_request_track(0x32);
            set_name_slot_from_text(0, lookup_with_two_overrides(tribe + 4));
            if (human)
                text_message_show("SCREWED", 3);
            tribe_hostile(tribe, g_turn_nation, 100, 0);
        }
    }
    switch (outcome) {
    case 1:
        set_current_nation(NATION(g_units[u]));
        for (i = 0; i < 8; i++)
            europe_recruit(1, 0);
        break;
    case 2:
        break;
    case 5:
        if (human) {
            music_mood_store_1040_8936(1);
            if (g_sound_music == 0)
                music_set_mood(1);
        }
        unit_record_free(u);
        redraw = 1;
        break;
    case 9:
        newu = create_unit_1038_2246(UNIT_COLONISTS, NATION(g_units[u]), x, y);
        if (newu < 0)
            goto done;
        unit_replace_at_self(u);
        break;
    }
    if (gold != 0)
        g_nations[g_turn_nation].gold += gold;
done:
    if (human && redraw) {
        redraw_map_area(x - 3, y - 3, 7, 7, !mapchange);
        if (mapchange != 0)
            empty_1040_a184(8);
    }
}
