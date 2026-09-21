/* chain_highest_flagged -- shipped at 1028:0aa7.
 *
 * Ported by tools/port.py from matched/game/chain_highest_flagged.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 unit_chain_next(col_i16 u);

col_i16 chain_highest_flagged(register col_i16 u)
{
    col_i16 best = -1;

    for (; u >= 0; u = unit_chain_next(u)) {
        if ((g_unit_types[g_units[u].type].bits & 0x40) != 0) {
            if (best < 0 || g_units[u].type > g_units[best].type)
                best = u;
        }
    }
    return best;
}
