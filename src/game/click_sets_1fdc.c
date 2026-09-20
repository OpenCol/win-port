/* click_sets_1fdc -- shipped at 1048:ae31.
 *
 * Ported by tools/port.py from matched/game/click_sets_1fdc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_flag_1fdc;                              DGROUP:0x1fdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int click_sets_1fdc(void)
{
 g_flag_1fdc = 1;
 return 1;
}
