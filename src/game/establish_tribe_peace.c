/* establish_tribe_peace -- shipped at 1030:377d.
 *
 * Ported by tools/port.py from matched/game/establish_tribe_peace.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"
#include "control.h"
#include "tribes.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern col_i16 nation_pair_flags_or_both(col_i16 a, col_i16 b, col_i16 mask);
extern col_i16 get_adjective(col_i16 nation);
extern col_i16 lookup_with_two_overrides(col_i16 n);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void tribe_message_show_1050_6443(char *msg, col_i16 tribe);
extern col_i16 table_78_word(col_i16 t, col_i16 i);

col_i16 establish_tribe_peace(col_i16 nation, col_i16 tribe)
{
    col_i16 r = nation_pair_flags_or_both(nation, tribe, 0x40);

    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
        set_name_slot_from_text(0, get_adjective(tribe));
        set_name_slot_from_text(1, get_adjective(nation));
        tribe_message_show_1050_6443("INDIANPEACE", tribe - 4);
        if (table_78_word(nation - 4, tribe) < TRIBE_ATTITUDE_MEDIUM) {
            set_name_slot_from_text(0, lookup_with_two_overrides(tribe));
            tribe_message_show_1050_6443("INDIANCOME", tribe - 4);
        }
    }
    return r;
}
