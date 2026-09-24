/* sell_treasure_to_agents -- shipped at 1020:420d.
 *
 * Ported by tools/port.py from matched/game/sell_treasure_to_agents.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern char far g_nation_text[][0x34];               SEG20:0x7810
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_difficulty_words[];                 SEG20:0xd8fe
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Unit, Nation dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: royal_gold -> f22, treasure -> spec
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1020_a887(col_i16 a, col_i16 b);
col_i16 min_int_1020_a8ad(col_i16 a, col_i16 b);
col_i16 show_text_dialog_1020_aa02(char *s);
void set_name_slot_1020_a94a(col_i16 i, char *s);
void set_text_arg_c(col_i16 i, char v);
void set_text_arg_i_1020_a926(col_i16 i, col_i16 v);
void set_text_arg_u_1020_a9de(col_i16 i, col_u16 v);

#include "fathers.h"
#include "gameflag.h"
#include "sounds.h"
/* 316 bytes at SEG20:0xc2f4 */







extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void set_nation_name_slot(col_i16 slot, col_i16 a, col_i16 nation);


extern void play_sound_effect(col_i16 n);
extern void text_message_show(char *msg, col_i16 n);
extern void music_mood_if_playing(col_i16 n);
extern col_i16 lookup_with_two_overrides(col_i16 n);
extern void unit_record_free(col_i16 u);

/* 5-byte NOP form, so extern (rule 47) */
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* same object: 4-byte `push cs; call ` */








void sell_treasure_to_agents(col_i16 u, col_i16 n)
{
    col_i16 r;
    col_i16 pct;
    col_i32 amount;
    char buf[0x50];
    register col_i16 nation;
    register col_u16 treasure;

    nation = n;
    treasure = g_units[u].spec;
    treasure *= 100;
    if ((g_game_flags & GAMEFLAG_INDEPENDENCE) != 0) {
        set_text_arg_u_1020_a9de(0, treasure);
        text_message_show("CASHTREASURE", 2);
        music_mood_if_playing(2);
    } else {
        set_name_slot_from_text(0, g_difficulty_words[g_difficulty]);
        set_name_slot_1020_a94a(1, (&g_nation_text[nation]));
        set_nation_name_slot(2, 0, nation);
        set_text_arg_c(0, g_nations[nation].tax);
        strcpy(buf, "KINGGALLEON");
        if (nation_flag_test(nation, FATHER_HERNAN_CORTES) != 0)
            strcat(buf, "3");
        else
            strcat(buf, "2");
        play_sound_effect(SOUND_MEETKING);
        r = show_text_dialog_1020_aa02(buf);
        if (r != 1)
            goto done;
        pct = g_nations[nation].tax;
        if (nation_flag_test(nation, FATHER_HERNAN_CORTES) == 0) {
            pct = pct * 2;
            pct = max_int_1020_a887(g_difficulty * 5 + 0x32, pct);
        }
        pct = min_int_1020_a8ad(0x5a, pct);
        amount = (col_i32)treasure * pct / 100;
        set_text_arg_u_1020_a9de(0, treasure);
        set_text_arg_i_1020_a926(1, pct);
        treasure -= (col_u16)amount;
        set_text_arg_u_1020_a9de(2, treasure);
        set_name_slot_from_text(0, lookup_with_two_overrides(nation));
        set_name_slot_from_text(1, g_nation_name_word[nation]);
        music_mood_if_playing(2);
        text_message_show("LOOTCASH", 2);
        g_nations[nation].f22 += amount;
    }
    g_nations[nation].gold += treasure;
    g_nations[nation].f26 += treasure;
    unit_record_free(u);
done:
    ;
}
