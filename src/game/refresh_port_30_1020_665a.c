/* refresh_port_30_1020_665a -- shipped at 1020:665a.
 *
 * Ported by tools/port.py from matched/game/refresh_port_30_1020_665a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_4bec;                                   DGROUP:0x4bec
 *   extern char far g_win[];                             SEG22:0x000a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void end_modal_1060_9fad(void *port);

void refresh_port_30_1020_665a(void)
{
    g_4bec = 1;
    end_modal_1060_9fad(g_win + 0x26);
}
