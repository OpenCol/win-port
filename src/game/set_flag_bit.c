/* set_flag_bit -- shipped at 1050:63ca.
 *
 * Ported by tools/port.py from matched/game/set_flag_bit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_flags_2514;                        DGROUP:0x2514
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_flag_bit(int bit, int on)
{
 if (on)
 g_flags_2514 |= 1 << bit;
 else
 g_flags_2514 &= ~(1 << bit);
}
