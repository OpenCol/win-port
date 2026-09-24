/* unit_join_nation_units -- shipped at 1038:2bc0.
 *
 * Ported by tools/port.py from matched/game/unit_join_nation_units.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nation_adjacent(col_i16 x, col_i16 y, col_i16 n);
void unit_chain_set_nation(col_i16 u, col_i16 n);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */






void unit_join_nation_units(col_i16 u)
{
    register col_i16 i;

    for (i = 0; i < 4; i++) {
        if (nation_adjacent(g_units[u].x, g_units[u].y, i) != 0)
            unit_chain_set_nation(u, i);
    }
}
