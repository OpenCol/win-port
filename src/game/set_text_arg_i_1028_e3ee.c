/* set_text_arg_i_1028_e3ee -- shipped at 1028:e3ee.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_i_1028_e3ee.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_i_1028_e3ee(int i, int v)
{
 g_text_args[i] = v;
}
