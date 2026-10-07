/* MSWaitTicks_1010_e5f6: candidate for 1010:e5f6 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSWaitTicks`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSWaitTicks_1010_e5f6(int ticks)
{
    MSWaitTicks(ticks);
}
