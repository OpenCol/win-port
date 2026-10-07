/* MSDisposeRadio: candidate for 1070:42aa (163 bytes).  Verdict: RELOCATABLE.
 *
 * Destroys the n button windows of a radio set (0x92-byte entries, hwnd first) and disposes
 * its handle; a set without a first window or a null handle is reported.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_969a(void *ctx, char *msg);

void MSDisposeRadio(int n, unsigned h)
{
    RadioEntry *r;
    int i;

    if (h != 0) {
        r = (RadioEntry *)MSHLock(h);
        if (r->hwnd != 0) {
            for (i = 0; i < n; i++)
                if (IsWindow(r[i].hwnd))
                    DestroyWindow(r[i].hwnd);
        } else {
            report_error_1070_969a(&g_error_log, "Error: Tried to dispose NULL RadioEntry in MSDisposeRadio");
        }
        MSHUnlock(h);
        MSDisposeHandle(h);
    } else {
        report_error_1070_969a(&g_error_log, "Error: Tried to dismantle NULL RadioSet in MSDisposeRadio");
    }
}
