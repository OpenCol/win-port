/* MSHLock: candidate for 1070:d073 (38 bytes).  Verdict: RELOCATABLE.
 *
 * GlobalLock on a non-null handle; null for a null one.
 */

#include "colonization.h"

void *MSHLock(int h)
{
    if (h != 0)
        return GlobalLock(h);
    else
        return 0;
}
