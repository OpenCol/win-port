/* unit_cargo_room -- shipped at 1018:7eba.
 *
 * Ported by tools/port.py from matched/game/unit_cargo_room.c.
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
col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
col_i16 unit_cargo_type(col_i16 u, col_i16 slot);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */






col_i16 unit_cargo_room(col_i16 u, col_i16 goods, col_i16 *room)
{
    register col_i16 k;
    col_i16 free;

    free = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
    *room = free * 100;
    if (free == 0) {
        for (k = 0; k < g_units[u].cargo_count; k++) {
            if (unit_cargo_type(u, k) == goods) {
                if (unit_cargo_amount(u, k) < 100) {
                    free++;
                    *room += 100 - unit_cargo_amount(u, k);
                }
            }
        }
    }
    return free;
}
