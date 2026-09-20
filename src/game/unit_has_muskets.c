/* unit_has_muskets -- shipped at 1038:313d.
 *
 * Ported by tools/port.py from matched/game/unit_has_muskets.c.
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


col_i16 unit_has_muskets(col_i16 u)
{
    return g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_ARTILLERY || g_units[u].type == UNIT_ARMED_BRAVES || g_units[u].type == UNIT_MOUNTED_WARRIORS;
}
