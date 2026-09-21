/* set_cd_done_cb -- shipped at 1068:a62f.
 *
 * Ported by tools/port.py from matched/gfx/set_cd_done_cb.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_cd_done_cb;                       DGROUP:0x569c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_cd_done_cb(void *fn)
{
    g_cd_done_cb = fn;
}
