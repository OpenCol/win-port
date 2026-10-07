/* min_long_1028_e39c: candidate for 1028:e39c (46 bytes).  Verdict: EXACT.
 *
 * The smaller of two longs; segment 1028's copy.
 */

#include "colonization.h"

long min_long_1028_e39c(long a, long b)
{
    if (a > b)
        return b;
    else
        return a;
}
