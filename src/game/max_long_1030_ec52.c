/* max_long_1030_ec52: candidate for 1030:ec52 (46 bytes).  Verdict: EXACT.
 *
 * The larger of two longs; segment 1030's copy.
 */

#include "colonization.h"

long max_long_1030_ec52(long a, long b)
{
    if (a < b)
        return b;
    else
        return a;
}
