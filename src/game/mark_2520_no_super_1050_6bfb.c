/* mark_2520_no_super_1050_6bfb -- shipped at 1050:6bfb.
 *
 * Ported by tools/port.py from matched/game/mark_2520_no_super_1050_6bfb.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_super_win_open;                         DGROUP:0x251e
 *   extern int g_flag_2520;                              DGROUP:0x2520
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void mark_2520_no_super_1050_6bfb(void)
{
    if (g_super_win_open == 0)
        g_flag_2520 = 1;
}
