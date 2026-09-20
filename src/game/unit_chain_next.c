/* unit_chain_next -- shipped at 1038:18ee.
 *
 * Ported by tools/port.py from matched/game/unit_chain_next.c.
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


col_i16 unit_chain_next(col_i16 u)
{
    if (u >= 0)
        u = g_units[u].link2;
    return u;
}
