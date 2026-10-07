/* MSDisposeHandle: candidate for 1070:d0f8 (60 bytes).  Verdict: RELOCATABLE.
 *
 * GlobalFree(h); a null handle is reported ("Error: Tried to dispose of NULL Handle in
 * MSDisposeHandle") and returns 0; else GlobalFree's result is returned. The census size
 * (53) stops inside the trailing dead epilogue; the function is 55 bytes -- and the
 * `jmp` over the else branch only resolves against the full extent.

 * The stated size was 55 and the function is 60: `tools/re_agent/check_sizes.py`
 * found the extra bytes decoding as an unreachable epilogue, and the reconstruction
 * already grades over the whole extent. Only the number moved.
 */

#include "colonization.h"

/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */
extern void report_error_1070_dc82(void *ctx, char *msg);   /* the module's 14-byte empty reporter */

int MSDisposeHandle(unsigned h)
{
    if (h != 0)
        return GlobalFree(h);
    else {
        report_error_1070_dc82(&g_error_log, "Error: Tried to dispose of NULL Handle in MSDisposeHandle");
        return 0;
    }
}
