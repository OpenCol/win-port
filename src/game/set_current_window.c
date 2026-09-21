/* set_current_window -- shipped at 1060:9fe6.
 *
 * Ported by tools/port.py from matched/game/set_current_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_current_window;                   DGROUP:0x2ae0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * set_current_window(void *w)
{
    void *old = g_current_window;
    g_current_window = w;
    return old;
}
