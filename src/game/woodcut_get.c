/* woodcut_get -- shipped at 1060:04c2.
 *
 * Ported by tools/port.py from matched/game/woodcut_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_bits_780c[];              SEG20:0x780c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 woodcut_get(col_i16 n)
{
    col_i16 idx;
    col_i16 mask;

    n--;
    idx = n >> 3;
    mask = 1 << (n & 7);
    return (unsigned char)(g_bits_780c[idx] & mask);
}
