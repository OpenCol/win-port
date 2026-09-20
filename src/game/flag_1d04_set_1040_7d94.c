/* flag_1d04_set_1040_7d94 -- shipped at 1040:7d94.
 *
 * Ported by tools/port.py from matched/game/flag_1d04_set_1040_7d94.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_flag_1d04;                              DGROUP:0x1d04
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void flag_1d04_set_1040_7d94(void)
{
    g_flag_1d04 = 1;
}
