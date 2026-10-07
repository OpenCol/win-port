/* MSGetNewTimer: candidate for 1078:190c (67 bytes).  Verdict: RELOCATABLE.
 *
 * SetTimer(hwnd, id, ms, NULL); 1 on success, else the error is reported and 0 returned.
 * The census size (61) stops inside the else branch; the function runs to 67.
 */

#include "colonization.h"

/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */
   /* report_error in stub_reporter.c */

int MSGetNewTimer(unsigned hwnd, unsigned ms, unsigned id)
{
    if (SetTimer(hwnd, id, ms, 0))
        return 1;
    else {
        stub_reporter(&g_error_log, "Error: Can't get new timer in MSGetNewTimer");
        return 0;
    }
}
