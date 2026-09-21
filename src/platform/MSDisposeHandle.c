/* MSDisposeHandle -- shipped at 1070:d0f8.
 *
 * Ported by tools/port.py from matched/platform/MSDisposeHandle.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_dc82(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GlobalFree(col_u16 h);
#endif

/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */
   /* the module's 14-byte empty reporter */


col_i16 MSDisposeHandle(col_u16 h)
{
    if (h != 0)
        return GlobalFree(h);
    else {
        empty_1070_dc82(&(*g_error_ctx), "Error: Tried to dispose of NULL Handle in MSDisposeHandle");
        return 0;
    }
}
