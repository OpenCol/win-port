/* min_long_1018_b3a3: candidate for 1018:b3a3 (46 bytes).  Verdict: EXACT.
 *
 * The smaller of two longs (segment 1018's copy).
 */

#include "colonization.h"

long min_long_1018_b3a3(long a, long b)
{
    if (a > b)
        return b;
    else
        return a;
}
