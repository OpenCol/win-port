/* closing_flag_set_1010_0403 -- shipped at 1010:0403.
 *
 * Ported by tools/port.py from matched/game/closing_flag_set_1010_0403.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_closing_flag;                           DGROUP:0x4b98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void closing_flag_set_1010_0403(void)
{
 g_closing_flag = 1;
}
