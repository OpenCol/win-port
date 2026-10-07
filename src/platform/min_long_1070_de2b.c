/* min_long_1070_de2b: candidate for 1070:de2b (46 bytes).  Verdict: EXACT.
 *
 * The smaller of two longs; segment 1070's copy.
 */

#include "colonization.h"

long min_long_1070_de2b(long a, long b)
{
    if (a > b)
        return b;
    else
        return a;
}
