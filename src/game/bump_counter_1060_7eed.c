/* bump_counter_1060_7eed -- shipped at 1060:7eed.
 *
 * Ported by tools/port.py from matched/game/bump_counter_1060_7eed.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_about_step;                             DGROUP:0x4e66
 *   extern int g_flag_4e64;                              DGROUP:0x4e64
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void bump_counter_1060_7eed(void)
{
    if (g_about_step < 0xf)
        g_about_step = 0x10;
    else
        g_flag_4e64 = 1;
}
