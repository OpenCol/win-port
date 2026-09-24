/* open_resource_dll -- shipped at 1078:00ef.
 *
 * Ported by tools/port.py from matched/platform/open_resource_dll.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern int g_resource_lib_count;                     DGROUP:0x3c76
 *   extern unsigned g_resource_libs[];                   DGROUP:0x56e6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void stub_reporter(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal LoadLibrary(const char *name);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */





col_u16 open_resource_dll(const char *name)
{
    char msg[128];
    col_u16 h = LoadLibrary(name);

    if (h == 0) {
        strcpy(msg, "Error: Resource file ");
        strcat(msg, name);
        strcat(msg, " not found.");
        stub_reporter(&(*g_error_ctx), msg);
        return 0;
    } else {
        g_resource_libs[g_resource_lib_count] = h;
        g_resource_lib_count++;
        return h;
    }
}
