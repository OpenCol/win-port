/* nation_gold -- shipped at 1018:402b.
 *
 * Ported by tools/port.py from matched/game/nation_gold.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 316 bytes at SEG20:0xc2f4 */


col_i32 nation_gold(col_i16 n)
{
    return g_nations[n].gold;
}
