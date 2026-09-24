/* set_value_and_overflow -- shipped at 1018:4869.
 *
 * Ported by tools/port.py from matched/game/set_value_and_overflow.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_value[];                            SEG21:0x0164
 *   extern int far g_over_lo[];                          SEG21:0x018c
 *   extern int far g_over_band[];                        SEG21:0x01b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_value_and_overflow(col_i16 i, col_i16 lo, col_i16 v, col_i16 span)
{
    g_value[i] = v;
    g_over_lo[i] = 0;
    g_over_band[i] = 0;
    if (v > lo)
        g_over_lo[i] = v - lo;
    if (lo + span < v)
        g_over_band[i] = v - (lo + span);
}
