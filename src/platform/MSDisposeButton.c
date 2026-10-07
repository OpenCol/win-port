/* MSDisposeButton: candidate for 1070:1d94 (39 bytes).  Verdict: RELOCATABLE.
 * symbol: MSDisposeButton
 *
 * Reports a null handle ("Error: Tried to dispose NULL Button in MSDisposeButton"); a live one needs nothing. The module's
 * empty reporter is defined here because the shipped call is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

extern void report_error_1070_969a(void *ctx, char *msg);

void MSDisposeButton(unsigned h)
{
    if (h != 0)
        ;
    else
        report_error_1070_969a(&g_error_log, "Error: Tried to dispose NULL Button in MSDisposeButton");
}
