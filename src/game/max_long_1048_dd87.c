/* max_long_1048_dd87: candidate for 1048:dd87 (46 bytes).  Verdict: EXACT.
 *
 * The larger of two longs, segment 1048's copy.
 */

#include "colonization.h"

long max_long_1048_dd87(long a, long b)
{
    if (a < b)
        return b;
    else
        return a;
}
