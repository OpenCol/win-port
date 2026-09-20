/* dialog_set_option_value -- shipped at 1048:db6a.
 *
 * Ported by tools/port.py from matched/game/dialog_set_option_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void dialog_set_option_value(int i, int v)
{
 g_text_args[i] = v;
}
