/* nation_defeated -- shipped at 1038:484e.
 *
 * Ported by tools/port.py from matched/game/nation_defeated.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_record_free(col_i16 u);

#include "control.h"
#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */





extern void nation_pair_flags_clear_both(col_i16 a, col_i16 b, col_i16 flag);
extern void nation_pair_flags_or_both(col_i16 a, col_i16 b, col_i16 flag);



void nation_defeated(col_i16 nation)
{
    register col_i16 u;

    nation_pair_flags_clear_both(nation, g_player_nation, 0xb);
    nation_pair_flags_clear_both(nation, g_mother_country_idx, 0xb);
    nation_pair_flags_or_both(nation, g_player_nation, 0x60);
    nation_pair_flags_or_both(nation, g_mother_country_idx, 0x60);
    for (u = g_unit_count - 1; u >= 0; u--) {
        if ((char)(g_units[u].flags & UNIT_NATION_MASK) != (char)nation)
            continue;
        unit_record_free(u);
    }
    g_nation52[nation].control = CONTROL_DEFEATED;
}
