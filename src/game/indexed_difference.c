/* indexed_difference -- shipped at 1018:47dd.
 *
 * Ported by tools/port.py from matched/game/indexed_difference.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_sel_00a6[];                        SEG21:0x00a6
 *   extern int  far g_prod[];                            SEG21:0x0122
 *   extern int  far g_value[];                           SEG21:0x0164
 *   extern int  far g_over_band[];                       SEG21:0x01b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 indexed_difference(col_i16 i, col_i16 *out)
{
    col_i16 n = g_prod[i] - g_value[i];

    if ((col_i16)g_sel_00a6[i] >= 0) {
        n -= g_over_band[g_sel_00a6[i]];
        if (out != 0)
            *out = g_over_band[g_sel_00a6[i]];
    }
    return n;
}
