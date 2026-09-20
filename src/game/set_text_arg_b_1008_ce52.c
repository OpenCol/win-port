/* set_text_arg_b_1008_ce52 -- shipped at 1008:ce52.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_b_1008_ce52.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_b_1008_ce52(col_i16 i, unsigned char v)
{
    g_text_args[i] = v;
}
