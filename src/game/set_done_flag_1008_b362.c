/* set_done_flag_1008_b362 -- shipped at 1008:b362.
 *
 * Ported by tools/port.py from matched/game/set_done_flag_1008_b362.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_done_flag;                          DGROUP:0x4b8c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_done_flag_1008_b362(void)
{
 g_done_flag = 1;
}
