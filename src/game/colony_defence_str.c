/* colony_defence_str -- shipped at 1038:4fa7.
 *
 * Ported by tools/port.py from matched/game/colony_defence_str.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_chain_next(col_i16 u);

#include "building.h"
#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

extern col_i16 unit_combat_strength(col_i16 u, col_i16 a);
extern col_i16 current_colony_flag_get(col_i16 bit);






col_i16 colony_defence_str(void)
{
    col_i16 u;
    register col_i16 s = 1;

    s += (g_cur_colony_rec->strength + 50) / 100;
    for (u = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = unit_chain_next(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            s += unit_combat_strength(u, 1) >> 4;
    }
    if (current_colony_flag_get(BUILDING_FORTRESS) != 0)
        s = s * 2;
    else if (current_colony_flag_get(BUILDING_FORT) != 0)
        s = (s * 2 + s) >> 1;
    s = max_int_1038_86a6(1, s);
    return s;
}
