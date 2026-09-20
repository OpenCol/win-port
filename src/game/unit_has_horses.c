/* unit_has_horses -- shipped at 1038:30d9.
 *
 * Ported by tools/port.py from matched/game/unit_has_horses.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


col_i16 unit_has_horses(col_i16 u)
{
    return g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_MOUNTED_BRAVES || g_units[u].type == UNIT_MOUNTED_WARRIORS;
}
