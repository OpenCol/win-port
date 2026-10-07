/* rand_range: candidate for 1040:751a (70 bytes).  Verdict: RELOCATABLE.
 *
 * A random integer in lo..hi: the runtime's rand() scaled by the span
 * through a 32-bit multiply.
 */

#include "colonization.h"

int rand_range(int lo, int hi)
{
    int span = hi - lo + 1;
    int r;

    r = (int)((long)rand_rtl() * span >> 15);
    return r + lo;
}
