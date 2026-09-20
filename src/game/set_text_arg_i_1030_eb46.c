/* set_text_arg_i_1030_eb46 -- shipped at 1030:eb46.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_i_1030_eb46.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_i_1030_eb46(int i, int v)
{
 g_text_args[i] = v;
}
