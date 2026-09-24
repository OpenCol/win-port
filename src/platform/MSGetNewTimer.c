/* MSGetNewTimer -- shipped at 1078:190c.
 *
 * Ported by tools/port.py from matched/platform/MSGetNewTimer.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void stub_reporter(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetTimer(col_u16 hwnd, col_u16 id, col_u16 ms, void *proc);
#endif

/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */
   /* report_error in stub_reporter.c */


col_i16 MSGetNewTimer(col_u16 hwnd, col_u16 ms, col_u16 id)
{
    if (SetTimer(hwnd, id, ms, 0))
        return 1;
    else {
        stub_reporter(&(*g_error_ctx), "Error: Can't get new timer in MSGetNewTimer");
        return 0;
    }
}
