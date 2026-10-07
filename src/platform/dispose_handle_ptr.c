/* dispose_handle_ptr: candidate for 1078:3316 (53 bytes).  Verdict: RELOCATABLE.
 *
 * Unlocks and disposes the memory handle the pointer holds, when both are
 * non-null.
 */

#include "colonization.h"

void dispose_handle_ptr(int *p)
{
    register int h;

    if (p != 0) {
        h = *p;
        if (h != 0) {
            MSHUnlock(h);
            h = MSDisposeHandle(h);
        }
    }
}
