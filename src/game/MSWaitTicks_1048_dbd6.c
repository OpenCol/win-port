/* MSWaitTicks_1048_dbd6: candidate for 1048:dbd6 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSWaitTicks`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSWaitTicks_1048_dbd6(int ticks)
{
    MSWaitTicks(ticks);
}
