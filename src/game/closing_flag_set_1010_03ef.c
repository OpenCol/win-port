/* closing_flag_set_1010_03ef -- shipped at 1010:03ef.
 *
 * Ported by tools/port.py from matched/game/closing_flag_set_1010_03ef.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_closing_flag;                           DGROUP:0x4b98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void closing_flag_set_1010_03ef(void)
{
 g_closing_flag = 1;
}
