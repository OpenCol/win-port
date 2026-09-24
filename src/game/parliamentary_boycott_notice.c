/* parliamentary_boycott_notice -- shipped at 1048:573d.
 *
 * Ported by tools/port.py from matched/game/parliamentary_boycott_notice.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *
 * The file's own NationRec52, Nation dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: royal_gold -> f22
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 goods_price_buy(col_i16 g);
void dialog_set_option_value(col_i16 slot, col_i16 v);
void set_text_arg_l_1048_db8e(col_i16 slot, col_i32 v);
void sub_1048_4e81(void);

#include "nations.h"
#include "control.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
/* same object: 4-byte `push cs; call ` (rule 47) */


extern col_i16 text_message_show(char *msg, col_i16 n);


col_i16 parliamentary_boycott_notice(col_i16 good)
{
    col_i16 r;
    col_i16 cost;
    col_i16 result = 0;

    if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_nation].control != CONTROL_HUMAN) {
        g_cur_nation_rec->boycott = 0;
        result = 1;
        goto done;
    }
    set_name_slot_from_text(0, g_goods_name_word[good]);
    set_name_slot_from_text(1, g_nation_name_word[g_cur_nation]);
    cost = goods_price_buy(good) * 500;
    dialog_set_option_value(0, cost);
    r = text_message_show("KISSUP", 2);
    if (r != 2)
        goto done;
    if ((col_i32)cost > g_cur_nation_rec->gold) {
        set_text_arg_l_1048_db8e(0, g_cur_nation_rec->gold);
        text_message_show("KISSSORRY", 2);
    } else {
        g_cur_nation_rec->gold -= cost;
        g_cur_nation_rec->f22 += cost;
        g_cur_nation_rec->boycott &= ~(1 << good);
        sub_1048_4e81();
    }
done:
    return result;
}
