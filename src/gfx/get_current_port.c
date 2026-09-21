/* get_current_port -- shipped at 1068:614a.
 *
 * Ported by tools/port.py from matched/gfx/get_current_port.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_current_port;                DGROUP:0x2ac2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_current_port(void)
{
    return g_current_port;
}
