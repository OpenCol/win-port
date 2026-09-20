/* set_text_arg_b_1030_eb6a -- shipped at 1030:eb6a.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_b_1030_eb6a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_b_1030_eb6a(int i, unsigned char v)
{
 g_text_args[i] = v;
}
