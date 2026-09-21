/* unit_add_cargo -- shipped at 1018:7d1c.
 *
 * Ported by tools/port.py from matched/game/unit_add_cargo.c.
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

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);
col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
col_i16 unit_set_cargo_type(col_i16 u, col_i16 slot, col_i16 type);
void unit_set_cargo_amount(col_i16 u, col_i16 slot, col_i16 amt);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */












col_i16 unit_add_cargo(col_i16 u, col_i16 goods, col_i16 amount)
{
    register col_i16 k;
    col_i16 count;
    col_i16 have;
    col_i16 room;
    col_i16 take;

    for (k = 0; k < g_units[u].cargo_count; k++) {
        if (unit_cargo_type(u, k) == goods) {
            have = unit_cargo_amount(u, k);
            room = 100 - have;
            if (room != 0) {
                take = min_int_1018_b329(room, amount);
                unit_set_cargo_amount(u, k, have + take);
                amount -= take;
            }
        }
    }
    if (amount != 0) {
        count = g_units[u].cargo_count;
        if (g_unit_types[g_units[u].type].holds > count) {
            unit_set_cargo_type(u, count, goods);
            unit_set_cargo_amount(u, count, amount);
            g_units[u].cargo_count++;
        }
    }
    return count;
}
