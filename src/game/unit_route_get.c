/* unit_route_get -- shipped at 1038:301b.
 *
 * Ported by tools/port.py from matched/game/unit_route_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: route -> spec
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


col_i16 unit_route_get(col_i16 u)
{
    return g_units[u].spec & 0xf;
}
