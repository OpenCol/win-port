/* set_ptr_2504 -- shipped at 1050:00e4.
 *
 * Ported by tools/port.py from matched/game/set_ptr_2504.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far *g_ptr_2504;                         DGROUP:0x2504
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_ptr_2504(char *p)
{
    g_ptr_2504 = p;
}
