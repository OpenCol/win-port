/* tribe_wagon_reaction -- shipped at 1030:c850.
 *
 * Ported by tools/port.py from matched/game/tribe_wagon_reaction.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 lookup_with_two_overrides(col_i16 nation);
void native_village_trade(col_i16 u, col_i16 a, col_i16 tribe, col_i16 nation);

extern col_i16 table_78_word(col_i16 a, col_i16 tribe);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void set_name_slot_from_text(col_i16 slot, col_i16 v);
extern void text_message_show(char *s, col_i16 n);
extern void tribe_message_show_1050_6443(char *s, col_i16 x);
extern void unit_record_free(col_i16 u);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */



col_i16 tribe_wagon_reaction(col_i16 unit, col_i16 a, col_i16 tribe, col_i16 nation)
{
    col_i16 result;
    col_i16 threshold;
    col_i16 roll;

    result = 0;
    threshold = table_78_word(g_cur_tribe, tribe);
    roll = rand_range(0, 0x1f4);
    set_name_slot_from_text(0, lookup_with_two_overrides(nation));
    if (roll <= threshold) {
        text_message_show("KILLWAGONS", 3);
        unit_record_free(unit);
        result = 1;
    } else if (threshold * 2 >= roll) {
        tribe_message_show_1050_6443("MADATWAGONS", g_cur_tribe);
    } else {
        tribe_message_show_1050_6443("GRUDGEWAGONS", g_cur_tribe);
        native_village_trade(unit, a, tribe, nation);
    }
    return result;
}
