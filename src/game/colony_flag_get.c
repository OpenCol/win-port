/* colony_flag_get -- shipped at 1018:3da3.
 *
 * Ported by tools/port.py from matched/game/colony_flag_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_colony_flags[][0xca];       SEG20:0x7cac
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int colony_flag_get(int colony, int bit)
{
 if (bit < 0)
 return 0;
 return g_colony_flags[colony][bit >> 3] & (1 << (bit & 7));
}
