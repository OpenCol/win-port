/* status_bits -- shipped at 1028:080e.
 *
 * Ported by tools/port.py from matched/game/status_bits.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_region_size[];                      SEG27:0x008c
 *   extern unsigned char far g_ba[];                     SEG20:0xda60
 *   extern unsigned char far g_navy_table[][0x10];       SEG20:0xdab0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 status_bits(col_i16 p1, col_i16 p2)
{
    col_i16 r = 0;
    col_i16 q;
    col_i16 t;
    col_i16 sum;
    col_i16 i;

    if (p2 < 0) goto done;
    q = g_region_size[p2] / 12;
    sum = g_ba[p2];
    for (i = 0; i < 4; i++)
        sum += g_navy_table[i][p2];
    t = (q - sum > 0) ? 1 : ((q - sum < 0) ? -1 : 0);
    r += t;
    if (sum - g_ba[p2] == 0)
        r += 2;
    if (g_navy_table[p1][p2] < 1)
        r += 4;
done:
    return r;
}
