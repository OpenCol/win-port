/* max_long_1018_b375: candidate for 1018:b375 (46 bytes).  Verdict: EXACT.
 *
 * The larger of two longs (segment 1018's copy).
 */

#include "colonization.h"

long max_long_1018_b375(long a, long b)
{
    if (a < b)
        return b;
    else
        return a;
}
