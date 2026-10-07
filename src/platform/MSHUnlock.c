/* MSHUnlock: candidate for 1070:d0a0 (38 bytes).  Verdict: RELOCATABLE.
 *
 * GlobalUnlock on a non-null handle; returns 0.
 */

#include "colonization.h"

void *MSHUnlock(int h)
{
    if (h != 0)
        GlobalUnlock(h);
    return 0;
}
