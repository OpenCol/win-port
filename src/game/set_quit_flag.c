/* set_quit_flag -- shipped at 1050:658f.
 *
 * Ported by tools/port.py from matched/game/set_quit_flag.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_quit_flag;                          DGROUP:0x4b04
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 set_quit_flag(void)
{
    g_quit_flag = 1;
    return 1;
}
