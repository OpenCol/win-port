/* click_sets_4ca6 -- shipped at 1040:4668.
 *
 * Ported by tools/port.py from matched/game/click_sets_4ca6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_flag_4ca6;                              DGROUP:0x4ca6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int click_sets_4ca6(void)
{
 g_flag_4ca6 = 1;
 return 1;
}
