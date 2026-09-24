/* recount_land_units_on_tile -- shipped at 1048:4c27.
 *
 * Ported by tools/port.py from matched/game/recount_land_units_on_tile.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern int g_land_unit_count;                        DGROUP:0x4cec
 *   extern int g_land_unit_index;                        DGROUP:0x4cee
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);

void recount_land_units_on_tile(void)
{
    register col_i16 u;

    g_land_unit_count = 0;
    for (u = unit_at(g_cur_nation - 20, g_cur_nation - 20); u >= 0;
         u = unit_chain_next(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            g_land_unit_count++;
    }
    if (g_land_unit_count != 0) {
        if (g_land_unit_index >= g_land_unit_count)
            g_land_unit_index = 0;
    }
}
