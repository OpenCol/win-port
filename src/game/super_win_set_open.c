/* super_win_set_open -- shipped at 1050:6be1.
 *
 * Ported by tools/port.py from matched/game/super_win_set_open.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_super_win_open;                         DGROUP:0x251e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void super_win_set_open(void *w, col_i16 open)
{
    if (open != 0)
        g_super_win_open = 1;
}
