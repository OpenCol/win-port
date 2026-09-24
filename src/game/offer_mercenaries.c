/* offer_mercenaries -- shipped at 1038:774c.
 *
 * Ported by tools/port.py from matched/game/offer_mercenaries.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far * far g_cur_nation_rec;            DGROUP:0x4cc4
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_merc_nation;                        SEG20:0x77d8
 *   extern int far g_merc[4];                            SEG26:0x0036
 *   extern int far g_word_dragoons;                      SEG20:0x7b00
 *   extern int far g_word_cavalry;                       SEG20:0x7b38
 *   extern int far g_word_artillery;                     SEG20:0x7b62
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void intervention_force_arrives(col_i16 n);
void set_name_slot_1038_8775(col_i16 slot, char *name);
void set_text_arg_i_1038_86cc(col_i16 slot, col_i16 v);

#include "royal.h"
#include "gameflag.h"
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern char nation_pair_flags_get(col_i16 a, col_i16 b);
extern void say_number(char *s, col_i16 v);
extern void say_string(char *s, col_i16 word);
extern void append_space(char *s);
extern void append_comma_space(char *s);
extern void set_nation_name_slot(col_i16 slot, col_i16 flag, col_i16 nation);


extern col_i16 text_message_show(char *msg, col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */

void offer_mercenaries(void)
{
    col_i16 r;
    char buf[0x50];
    col_i16 i;
    col_i16 price;

    if (g_game_flags & GAMEFLAG_INDEPENDENCE)
        return;
    if (rand_range(0, 20) != 0)
        return;
    g_merc_nation = rand_range(0, 3);
    if (g_merc_nation != g_player_nation && (nation_pair_flags_get(g_merc_nation, g_player_nation) & 0x40) == 0)
        return;
    for (i = 0; i < 4; i++)
        g_merc[i] = 0;
    g_merc[ROYAL_REGULARS] = rand_range(1, 3);
    if (rand_range(0, 1) != 0)
        g_merc[ROYAL_REGULARS]++;
    else {
        g_merc[ROYAL_ARTILLERY] = 1;
        if (rand_range(0, 1) == 0)
            g_merc[ROYAL_ARTILLERY]++;
    }
    price = rand_range(0, 6) * 100 + g_difficulty * 200 + 800;
    price = ((g_merc[ROYAL_CAVALRY] + g_merc[ROYAL_ARTILLERY]) * 2 + g_merc[ROYAL_REGULARS]) * price;
    buf[0] = 0;
    say_number(buf, g_merc[ROYAL_REGULARS]);
    append_space(buf);
    say_string(buf, g_word_dragoons);
    if (g_merc[ROYAL_CAVALRY] != 0) {
        append_comma_space(buf);
        say_string(buf, g_word_cavalry);
    }
    if (g_merc[ROYAL_ARTILLERY] != 0) {
        append_comma_space(buf);
        if (g_merc[ROYAL_ARTILLERY] > 1) {
            say_number(buf, g_merc[ROYAL_ARTILLERY]);
            append_space(buf);
        }
        say_string(buf, g_word_artillery);
    }
    if ((col_i32)price <= g_cur_nation_rec->gold) {
        set_nation_name_slot(0, 0, g_merc_nation);
        set_text_arg_i_1038_86cc(0, price);
        set_name_slot_1038_8775(1, buf);
        r = text_message_show("MERCENARIES", 1);
        if (r == 2) {
            g_cur_nation_rec->gold -= price;
            intervention_force_arrives(1);
        }
    }
}
