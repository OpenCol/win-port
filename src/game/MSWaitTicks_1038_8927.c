/* MSWaitTicks_1038_8927: candidate for 1038:8927 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSWaitTicks`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSWaitTicks_1038_8927(int ticks)
{
    MSWaitTicks(ticks);
}
