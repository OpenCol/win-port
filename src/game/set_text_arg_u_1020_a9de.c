/* set_text_arg_u_1020_a9de -- shipped at 1020:a9de.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_u_1020_a9de.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_u_1020_a9de(col_i16 i, col_u16 v)
{
    g_text_args[i] = v;
}
