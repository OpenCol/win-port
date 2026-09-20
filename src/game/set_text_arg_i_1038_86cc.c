/* set_text_arg_i_1038_86cc -- shipped at 1038:86cc.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_i_1038_86cc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_i_1038_86cc(col_i16 i, col_i16 v)
{
    g_text_args[i] = v;
}
