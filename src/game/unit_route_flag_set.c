/* unit_route_flag_set -- shipped at 1038:30a0.
 *
 * Ported by tools/port.py from matched/game/unit_route_flag_set.c.
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


void unit_route_flag_set(col_i16 u, col_i16 n)
{
    g_units[u].spec = (g_units[u].spec & 0xf) | (unsigned char)(n << 4);
}
