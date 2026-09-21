/* load_resource_library -- shipped at 1070:b13e.
 *
 * Ported by tools/port.py from matched/platform/load_resource_library.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_resource_library;                  DGROUP:0x56e0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal LoadLibrary(const char *name);;
#endif

void load_resource_library(void)
{
    g_resource_library = LoadLibrary("WinG.dll");
}
