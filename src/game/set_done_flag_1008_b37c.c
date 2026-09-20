/* set_done_flag_1008_b37c -- shipped at 1008:b37c.
 *
 * Ported by tools/port.py from matched/game/set_done_flag_1008_b37c.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_done_flag;                          DGROUP:0x4b8c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_done_flag_1008_b37c(void)
{
 g_done_flag = 1;
}
