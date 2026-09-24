/* nth_land_unit -- shipped at 1048:4abc.
 *
 * Ported by tools/port.py from matched/game/nth_land_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);


col_i16 nth_land_unit(col_i16 n)
{
    register col_i16 u;
    register col_i16 i = -1;
    col_i16 found = -1;

    for (u = unit_at(g_cur_nation - 0x14, g_cur_nation - 0x14); u >= 0;
         u = unit_chain_next(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
            i++;
            if (i == n)
                found = u;
        }
    }
    return found;
}
