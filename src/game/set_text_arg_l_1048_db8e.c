/* set_text_arg_l_1048_db8e -- shipped at 1048:db8e.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_l_1048_db8e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_l_1048_db8e(col_i16 i, col_i32 v)
{
    g_text_args[i] = v;
}
