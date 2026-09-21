/* swap_current_port -- shipped at 1068:610c.
 *
 * Ported by tools/port.py from matched/gfx/swap_current_port.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_current_port;                DGROUP:0x2ac2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * swap_current_port(void *p)
{
    void *old = g_current_port;

    g_current_port = p;
    return old;
}
