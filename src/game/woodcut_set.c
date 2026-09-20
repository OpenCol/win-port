/* woodcut_set -- shipped at 1060:046f.
 *
 * Ported by tools/port.py from matched/game/woodcut_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_bits_780c[];              SEG20:0x780c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void woodcut_set(int n, int on)
{
 register int idx;
 int mask;

 n--;
 idx = n >> 3;
 mask = 1 << (n & 7);
 if (on != 0)
 g_bits_780c[idx] |= mask;
 else
 g_bits_780c[idx] &= ~mask;
}
