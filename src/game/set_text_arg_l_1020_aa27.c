/* set_text_arg_l_1020_aa27 -- shipped at 1020:aa27.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_l_1020_aa27.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_l_1020_aa27(col_i16 i, col_i32 v)
{
    g_text_args[i] = v;
}
