/* set_ptr_24f0_buf -- shipped at 1050:00cc.
 *
 * Ported by tools/port.py from matched/game/set_ptr_24f0_buf.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_font16[];                              DGROUP:0x24e8
 *   extern char far *g_ptr_24f0;                         DGROUP:0x24f0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_ptr_24f0_buf(void)
{
    g_ptr_24f0 = g_font16;
}
