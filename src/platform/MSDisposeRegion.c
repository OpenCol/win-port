/* MSDisposeRegion: candidate for 1070:0c99 (39 bytes).  Verdict: RELOCATABLE.
 * symbol: MSDisposeRegion
 *
 * Reports a null handle ("Error: Tried to dispose NULL region in MSDisposeRegion"); a live one needs nothing. The module's
 * empty reporter is defined here because the shipped call is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

extern void report_error_1070_969a(void *ctx, char *msg);

void MSDisposeRegion(unsigned h)
{
    if (h != 0)
        ;
    else
        report_error_1070_969a(&g_error_log, "Error: Tried to dispose NULL region in MSDisposeRegion");
}
