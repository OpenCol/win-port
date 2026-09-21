/* current_window -- shipped at 1060:9fcd.
 *
 * Ported by tools/port.py from matched/game/current_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_current_window;                   DGROUP:0x2ae0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * current_window(void)
{
    return g_current_window;
}
