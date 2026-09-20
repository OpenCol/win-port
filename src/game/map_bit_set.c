/* map_bit_set -- shipped at 1020:97fa.
 *
 * Ported by tools/port.py from matched/game/map_bit_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_nation_bits[];                     SEG20:0xc2fb
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void map_bit_set(col_i16 row, col_i16 col, col_i16 on)
{
    col_i16 byte = col >> 3;
    col_i16 mask = 1 << (col & 7);
    char *p = &((unsigned char *)g_nation_bits)[row * 0x13c + byte];
    if (on)
        *p |= mask;
    else
        *p &= ~mask;
}
