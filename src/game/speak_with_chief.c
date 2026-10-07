/* speak_with_chief -- shipped at 1030:cde2.
 *
 * Ported by tools/port.py from matched/game/speak_with_chief.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Settlement far *g_cur_settlement_rec;         DGROUP:0x4c2c
 *   extern Tribe far *g_cur_tribe_rec;                   DGROUP:0x4c28
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *   extern int g_cur_settlement;                         DGROUP:0x4c34
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *   extern LevelRec far g_level[];                       SEG20:0xcb7c
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_native_buy[];                       SEG25:0x0010
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Unit, Settlement, Tribe, Nation, NationRec52, SpecRec, LevelRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 learn_from_natives(col_i16 u, col_i16 nation, col_i16 named, col_i16 ask);
col_i16 lookup_with_two_overrides(col_i16 n);
void native_village_trade(col_i16 u, col_i16 settlement, col_i16 a, col_i16 named);
void set_text_arg_i_1030_eb46(col_i16 slot, col_i16 v);

#include "fathers.h"
#include "control.h"
#include "settle.h"
#include "nations.h"
#include "jobs.h"
#include "sounds.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern col_i16 table_78_word(col_i16 t, col_i16 i);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void sort_int_pairs_by_key(col_i16 n, col_i16 *ids, col_i16 *keys);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void tribe_message_show_1050_6443(char *msg, col_i16 tribe);
extern void redraw_map_1040_11b7(col_i16 full);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void map_goto(col_i16 x, col_i16 y, col_i16 flag);
extern void explore_area(col_i16 u, col_i16 radius);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
extern void play_sound_effect(col_i16 n);
extern void unit_record_free(col_i16 u);
/* same object: 4-byte `push cs; call ` (rule 47) */





col_i16 speak_with_chief(col_i16 u, col_i16 nation_arg, col_i16 named)
{
    register col_i16 nation = nation_arg;
    register col_i16 k;
    col_i16 killed;
    col_i16 scout;
    col_i16 gift;
    col_i16 alarm;
    col_i16 roll;
    col_i16 skill;
    col_i16 i;
    col_i16 idx[16];

    killed = 0;
    scout = (col_i16)g_units[u].spec == JOB_SCOUT ? 1 : 0;
    alarm = table_78_word(g_cur_tribe, nation);
    if (alarm >= 0x4b)
        goto kill;
    k = scout * 40 + 100;
    roll = rand_range(0, k);
    if (alarm >= 0x19 && alarm >> 2 >= roll)
        goto kill;
    if (g_cur_tribe == NATION_ARAWAKS - NATION_INCAS) {
        k = (8 - g_difficulty) << scout;
        if (rand_range(0, k) == 0)
            goto kill;
    }
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
        skill = learn_from_natives(u, nation, named, 1);
        native_village_trade(u, g_cur_settlement, -1, named);
        for (i = 0; i < 16; i++)
            idx[i] = (unsigned char)i;
        if ((col_i16)g_cur_settlement_rec->sold >= 0)
            g_native_buy[(col_i16)g_cur_settlement_rec->sold] = 0;
        if ((col_i16)g_cur_settlement_rec->bought >= 0)
            g_native_buy[(col_i16)g_cur_settlement_rec->bought] = 0;
        sort_int_pairs_by_key(16, idx, g_native_buy);
        set_name_slot_from_text(0, g_spec_recs[skill].expert);
        set_name_slot_from_text(1, g_goods_name_word[idx[15]]);
        set_name_slot_from_text(2, g_goods_name_word[idx[14]]);
        set_name_slot_from_text(3, g_goods_name_word[idx[13]]);
        tribe_message_show_1050_6443("CHIEFHOWDY", g_cur_tribe);
    }
    set_name_slot_from_text(0, lookup_with_two_overrides(named));
    if (roll <= alarm)
        goto bored;
    if (g_cur_settlement_rec->flags & SETTLEMENT_CHIEF_SPOKE)
        goto bored;
    g_cur_settlement_rec->flags |= SETTLEMENT_CHIEF_SPOKE;
    k = rand_range(1, 3);
    switch (k) {
    case 1:
        if (scout == 0) {
            set_name_slot_from_text(1, g_level[g_cur_tribe_rec->level].settlement);
            g_units[u].spec = JOB_SCOUT;
            if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
                tribe_message_show_1050_6443("CHIEFGUIDES", g_cur_tribe);
                redraw_map_1040_11b7(1);
                text_message_show("WELLSEASONED", 3);
            }
            goto done;
        }
    case 2:
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
            map_goto(g_units[u].x, g_units[u].y, 0);
            tribe_message_show_1050_6443("CHIEFAREA", g_cur_tribe);
        }
        explore_area(u, 6);
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
            redraw_map_1040_11b7(0);
        goto done;
    case 3:
        set_name_slot_from_text(1, lookup_with_two_overrides(nation));
        k = 10 - g_difficulty;
        gift = rand_range(1, k) + rand_range(1, k) + rand_range(1, k);
        gift = gift * 4 * rand_range(1, 6);
        gift = (g_cur_tribe_rec->level + 1) * gift;
        set_text_arg_i_1030_eb46(0, gift);
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
            tribe_message_show_1050_6443("CHIEFGIFT", g_cur_tribe);
        g_nations[nation].gold += gift;
        goto done;
    default:
        goto bored;
    }
kill:
    if (nation_flag_test(nation, FATHER_FRANCISCO_DE_CORONADO)) /* by the father-bit rule FATHER_FRANCISCO_DE_CORONADO: see the header */
        goto bored;
    killed = 1;
    set_name_slot_from_text(0, lookup_with_two_overrides(named));
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
        play_sound_effect(SOUND_ARROW);
        tribe_message_show_1050_6443("CHIEFKILL", g_cur_tribe);
    }
    unit_record_free(u);
    goto done;
bored:
    set_name_slot_from_text(1, lookup_with_two_overrides(nation));
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
        tribe_message_show_1050_6443("CHIEFBORED", g_cur_tribe);
    goto done;
done:
    return killed;
}
