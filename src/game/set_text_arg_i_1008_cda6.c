/* set_text_arg_i_1008_cda6 -- shipped at 1008:cda6.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_i_1008_cda6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_i_1008_cda6(int i, int v)
{
 g_text_args[i] = v;
}
