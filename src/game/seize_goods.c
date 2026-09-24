/* seize_goods -- shipped at 1038:4df4.
 *
 * Ported by tools/port.py from matched/game/seize_goods.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern char far g_mother_country_idx;                SEG20:0x77d4
 *
 * The file's own Unit, NationRec52, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_record_free(col_i16 u);

#include "control.h"
#include "units.h"
/* 28 bytes at SEG20:0xa208 */

#define NATION(u) ((unsigned char)((u).flags & 0xf))
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

/* 14 bytes at SEG20:0x7ac8 */



extern void set_name_slot_from_text(col_i16 slot, col_i16 v);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern void text_message_show(char *msg, col_i16 n);



void seize_goods(unsigned char x, unsigned char y)
{
    register col_i16 u;

    for (u = g_unit_count - 1; u >= 0; u--) {
        if (g_units[u].x != x)
            continue;
        if (g_units[u].y != y)
            continue;
        if ((char)NATION(g_units[u]) == g_mother_country_idx)
            continue;
        if (NATION(g_units[u]) < 4 && g_nation52[NATION(g_units[u])].control == CONTROL_HUMAN) {
            set_name_slot_from_text(0, g_unit_types[g_units[u].type].name);
            if (ocean(g_units[u].x, g_units[u].y)) {
                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                    text_message_show("SEIZURESEA", 0);
            } else {
                text_message_show("SEIZURELAND", 1);
            }
        }
        unit_record_free(u);
    }
}
