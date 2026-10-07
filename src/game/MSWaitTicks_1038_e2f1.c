/* MSWaitTicks_1038_e2f1: candidate for 1038:e2f1 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSWaitTicks`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSWaitTicks_1038_e2f1(int ticks)
{
    MSWaitTicks(ticks);
}
