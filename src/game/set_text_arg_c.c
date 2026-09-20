/* set_text_arg_c -- shipped at 1020:a973.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_c(col_i16 i, char v)
{
    g_text_args[i] = (col_i32)v;
}
