/* destroy_module_gamewin_1020_b3ac -- shipped at 1020:b3ac.
 *
 * Ported by tools/port.py from matched/game/destroy_module_gamewin_1020_b3ac.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_win[];                             SEG22:0x000a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GameWin_dtor_1020_aac1(void *p, col_i16 flags);

/* same object: 4-byte `push cs; call ` */


void destroy_module_gamewin_1020_b3ac(void)
{
    GameWin_dtor_1020_aac1(g_win, 2);
}
