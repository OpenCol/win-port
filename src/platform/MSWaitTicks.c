/* MSWaitTicks: candidate for 1078:184a (59 bytes, 8 callers).  Verdict: RELOCATABLE.
 *
 * Busy-waits n ticks: spins on MSGetTicks until it reaches the start plus n.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void MSWaitTicks(int ticks)
{
    long t = MSGetTicks();

    while (MSGetTicks() < ticks + t)
        ;
}
