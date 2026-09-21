/* nth_colonist_unit -- shipped at 1018:456c.
 *
 * Ported by tools/port.py from matched/game/nth_colonist_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern signed char far g_unit_type_class[];          SEG21:0x010a
 *   extern int g_tile_first_unit;                        DGROUP:0x4bb8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_is_colonist(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */



extern col_i16 unit_chain_next(col_i16 u);



col_i16 nth_colonist_unit(col_i16 n)
{
    col_i16 found = -1;
    register col_i16 i = -1;
    register col_i16 u;

    for (u = g_tile_first_unit; u >= 0 && found < 0; u = unit_chain_next(u)) {
        if (unit_is_colonist(u) != 0) {
            i++;
            if (i == n)
                found = u;
        }
    }
    return found;
}
