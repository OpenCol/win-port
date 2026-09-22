/* MSInitMenu -- shipped at 1070:d5e6.
 *
 * Ported by tools/port.py from matched/platform/MSInitMenu.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal LoadMenu(col_u16 hinst, const char *name);
#endif

col_u16 MSInitMenu(col_u16 id)
{
    return LoadMenu(g_hinstance, (const char *)(col_u32)id);
}
