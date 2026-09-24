/* mission_burned -- shipped at 1038:022f.
 *
 * Ported by tools/port.py from matched/game/mission_burned.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int g_stub_a;                                 
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void burn_mission(col_i16 tribe, col_i16 nation);
void show_text_dialog_1038_865b(void *w);

#include "control.h"
#include "nations.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */


extern col_i16 get_adjective(col_i16 n);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void nation_pair_flags_clear_both(col_i16 a, col_i16 b, col_i16 flag);




void mission_burned(void *w, col_i16 tribe, col_i16 nation)
{
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN && w != 0) {
        set_name_slot_from_text(0, get_adjective(tribe + 4));
        show_text_dialog_1038_865b(w);
    }
    nation_pair_flags_clear_both(tribe + 4, nation, 0x40);
    burn_mission(tribe, nation);
}
