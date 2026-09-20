/* set_56b4 -- shipped at 1068:cbb8.
 *
 * Ported by tools/port.py from matched/gfx/set_56b4.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_dialog_result;                          DGROUP:0x56b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_56b4(col_i16 a, col_i16 b)
{
    g_dialog_result = b;
}
