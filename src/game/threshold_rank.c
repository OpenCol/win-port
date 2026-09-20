/* threshold_rank -- shipped at 1020:9884.
 *
 * Ported by tools/port.py from matched/game/threshold_rank.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_year;                               SEG20:0x778c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 threshold_rank(void)
{
    col_i16 n = 0;
    if (g_year >= 0x640) n++;
    if (g_year >= 0x6a4) n++;
    return n;
}
