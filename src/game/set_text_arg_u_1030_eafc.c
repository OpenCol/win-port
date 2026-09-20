/* set_text_arg_u_1030_eafc -- shipped at 1030:eafc.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_u_1030_eafc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_u_1030_eafc(col_i16 i, col_u16 v)
{
    g_text_args[i] = v;
}
