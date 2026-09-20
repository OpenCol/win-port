/* unit_is_colonist -- shipped at 1018:44e5.
 *
 * Ported by tools/port.py from matched/game/unit_is_colonist.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern signed char far g_unit_type_class[];          SEG21:0x010a
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */



col_i16 unit_is_colonist(col_i16 u)
{
    return (col_i16)g_unit_type_class[g_units[u].type] >= 0;
}
