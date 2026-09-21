/* close_resource_dll -- shipped at 1078:03b6.
 *
 * Ported by tools/port.py from matched/platform/close_resource_dll.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_resource_libs[];                   DGROUP:0x56e6
 *   extern unsigned g_resource_lib_count;                DGROUP:0x3c76
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal FreeLibrary(col_u16 h);
#endif

void close_resource_dll(col_u16 h)
{
    col_u16 i;

    for (i = 0; g_resource_libs[i] != h && i < g_resource_lib_count; i++)
        ;
    if (i < g_resource_lib_count) {
        for (; i < g_resource_lib_count; i++)
            g_resource_libs[i] = *(g_resource_libs + i + 1);
        g_resource_lib_count--;
        FreeLibrary(h);
    }
}
