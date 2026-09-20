/* set_text_arg_u_1028_e3ca -- shipped at 1028:e3ca.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_u_1028_e3ca.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_u_1028_e3ca(int i, unsigned v)
{
 g_text_args[i] = v;
}
