/* min_int_1030_eb20: candidate for 1030:eb20 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1030's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1030_eb20(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
