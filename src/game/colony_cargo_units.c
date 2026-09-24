/* colony_cargo_units -- shipped at 1018:7af0.
 *
 * Ported by tools/port.py from matched/game/colony_cargo_units.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);

col_i16 colony_cargo_units(void)
{
    register col_i16 u;
    register col_i16 n = 0;

    for (u = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = unit_chain_next(u)) {
        if (g_unit_types[g_units[u].type].holds != 0)
            n++;
    }
    return n;
}
