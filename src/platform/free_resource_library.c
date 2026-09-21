/* free_resource_library -- shipped at 1070:b158.
 *
 * Ported by tools/port.py from matched/platform/free_resource_library.c.
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
extern void pascal FreeLibrary(col_u16 h);
#endif

void free_resource_library(void)
{
    FreeLibrary(g_resource_library);
}
