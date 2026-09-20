/* unit_cargo_amount -- shipped at 1018:7c5a.
 *
 * Ported by tools/port.py from matched/game/unit_cargo_amount.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


col_i16 unit_cargo_amount(col_i16 u, col_i16 slot)
{
    return ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[slot];
}
