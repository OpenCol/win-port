/* burn_mission -- shipped at 1038:0162.
 *
 * Ported by tools/port.py from matched/game/burn_mission.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"
#include "control.h"
#include "settle.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

typedef struct { char pad[2]; unsigned char owner; char pad3[2]; signed char mission; char rest[0x12 - 6]; } Settlement_local;


extern col_i16 get_adjective(col_i16 nation);
extern col_i16 lookup_with_two_overrides(col_i16 n);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void text_message_show(char *msg, col_i16 n);

void burn_mission(col_i16 tribe, col_i16 nation)
{
    col_i16 burned = 0;
    col_i16 i;

    for (i = 0; g_settlement_count > i; i++) {
        if (g_settlements[i].owner == (unsigned char)(tribe + 4) && (g_settlements[i].mission & MISSION_NATION_MASK) == nation) {
            burned = 1;
            g_settlements[i].mission = 0xff;
        }
    }
    if (burned) {
        set_name_slot_from_text(0, get_adjective(tribe + 4));
        set_name_slot_from_text(1, lookup_with_two_overrides(nation));
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
            text_message_show("INDIANBURN", 1);
    }
}
