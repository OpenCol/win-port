/* MSWaitTicks_1020_a9c5: candidate for 1020:a9c5 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSWaitTicks`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSWaitTicks_1020_a9c5(int ticks)
{
    MSWaitTicks(ticks);
}
