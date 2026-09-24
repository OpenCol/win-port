/* MSDisposeRadio -- shipped at 1070:42aa.
 *
 * Ported by tools/port.py from matched/platform/MSDisposeRadio.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own RadioEntry dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_969a(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DestroyWindow(col_u16 hwnd);
extern col_i16 pascal IsWindow(col_u16 hwnd);
#endif

extern RadioEntry * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);


/* same object: 4-byte `push cs; call ` (rule 47) */



void MSDisposeRadio(col_i16 n, col_u16 h)
{
    RadioEntry *r;
    col_i16 i;

    if (h != 0) {
        r = MSHLock(h);
        if (r->hwnd != 0) {
            for (i = 0; i < n; i++)
                if (IsWindow(r[i].hwnd))
                    DestroyWindow(r[i].hwnd);
        } else {
            empty_1070_969a(&(*g_error_ctx), "Error: Tried to dispose NULL RadioEntry in MSDisposeRadio");
        }
        MSHUnlock(h);
        MSDisposeHandle(h);
    } else {
        empty_1070_969a(&(*g_error_ctx), "Error: Tried to dismantle NULL RadioSet in MSDisposeRadio");
    }
}
