/* min_long_1020_a997: candidate for 1020:a997 (46 bytes).  Verdict: EXACT.
 *
 * The smaller of two longs; segment 1020's copy.
 */

#include "colonization.h"

long min_long_1020_a997(long a, long b)
{
    if (a > b)
        return b;
    else
        return a;
}
