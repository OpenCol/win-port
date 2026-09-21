/* set_ptr_24f0 -- shipped at 1050:00b6.
 *
 * Ported by tools/port.py from matched/game/set_ptr_24f0.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far *g_ptr_24f0;                         DGROUP:0x24f0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_ptr_24f0(char *p)
{
    g_ptr_24f0 = p;
}
