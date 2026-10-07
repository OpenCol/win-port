/* min_int_1038_8680: candidate for 1038:8680 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1038's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1038_8680(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
