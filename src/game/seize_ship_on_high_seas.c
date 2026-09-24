/* seize_ship_on_high_seas -- shipped at 1038:48fe.
 *
 * Ported by tools/port.py from matched/game/seize_ship_on_high_seas.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_record_free(col_i16 u);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */



extern col_i16 on_map(col_i16 x, col_i16 y);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 text_message_show(char *msg, col_i16 n);



void seize_ship_on_high_seas(col_i16 nation)
{
    register col_i16 u;

    for (u = g_unit_count - 1; u >= 0; u--) {
        if ((char)(g_units[u].flags & UNIT_NATION_MASK) != (char)nation)
            continue;
        if (on_map(g_units[u].x, g_units[u].y) == 0) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                set_name_slot_from_text(0, g_unit_types[g_units[u].type].name);
                text_message_show("SEIZURE", 0);
            }
            unit_record_free(u);
        }
    }
}
