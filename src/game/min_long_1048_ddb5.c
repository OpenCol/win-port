/* min_long_1048_ddb5: candidate for 1048:ddb5 (46 bytes).  Verdict: EXACT.
 *
 * The smaller of two longs, segment 1048's copy.
 */

#include "colonization.h"

long min_long_1048_ddb5(long a, long b)
{
    if (a > b)
        return b;
    else
        return a;
}
