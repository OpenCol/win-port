/* chain_count_flagged -- shipped at 1048:49a0.
 *
 * Ported by tools/port.py from matched/game/chain_count_flagged.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);

col_i16 chain_count_flagged(col_i16 *out)
{
    col_i16 u;
    col_i16 n = 0;

    *out = 0;
    for (u = unit_at(g_cur_nation - 20, g_cur_nation - 20); u >= 0; u = unit_chain_next(u)) {
        if (g_unit_types[g_units[u].type].holds != 0) {
            if ((g_units[u].f4 & 0x80) && g_units[u].type != UNIT_ARTILLERY)
                (*out)++;
            n++;
        }
    }
    return n;
}
