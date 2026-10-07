/* min_int_1068_9847: candidate for 1068:9847 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1068's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1068_9847(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
