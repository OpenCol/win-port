/* set_text_arg_1050_920a -- shipped at 1050:920a.
 *
 * Ported by tools/port.py from matched/game/set_text_arg_1050_920a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_text_arg_1050_920a(int i, int v)
{
 g_text_args[i] = v;
}
