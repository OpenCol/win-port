/* foreign_attack_revolution_check -- shipped at 1020:969e.
 *
 * Ported by tools/port.py from matched/game/foreign_attack_revolution_check.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitTypeInfo far g_unit_type_info[];          SEG20:0x7acf
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern unsigned char far g_mother_country_idx;       SEG20:0x77d4
 *
 * The file's own Unit, UnitTypeInfo, NationRec52, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 foreign_colony(col_i16 u);
col_i16 scout_enters_colony(col_i16 u);
void show_text_dialog_1020_aa02(char *msg);

#include "control.h"
#include "nations.h"
#include "gameflag.h"
#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern void set_current_colony(col_i16 c);
/* same object: 4-byte `push cs; call ` (rule 47) */



extern void unit_refresh_move_points(col_i16 u);

col_i16 foreign_attack_revolution_check(col_i16 u, col_i16 colony)
{
    col_i16 count;
    col_i16 r = 0;

    count = g_unit_count;
    set_current_colony(colony);
    if (g_units[u].type == UNIT_SCOUTS)
        r = scout_enters_colony(u);
    else if (g_unit_type_info[g_units[u].type].f0 != 0)
        r = foreign_colony(u);
    else
        ;
    if (g_unit_count != count)
        return r;
    if ((g_game_flags & GAMEFLAG_INDEPENDENCE) && r == 0
        && (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
        && g_cur_colony_rec->nation != g_mother_country_idx
        && (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) < 4
        && g_nation52[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)].control == CONTROL_HUMAN) {
        show_text_dialog_1020_aa02("NOWARSDURINGREV");
        r = 1;
    }
    if (r != 0)
        unit_refresh_move_points(u);
    return r;
}
