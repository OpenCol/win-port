/* nation_flag_test -- shipped at 1020:a82e.
 *
 * Ported by tools/port.py from matched/game/nation_flag_test.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_bits[][0x13c];     SEG20:0xc2fb
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int nation_flag_test(int nation, int bit)
{
 if (bit < 0)
 return 1;
 if (nation >= 4)
 return 0;
 return g_nation_bits[nation][bit >> 3] & (1 << (bit & 7));
}
