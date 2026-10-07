/* min_int_1020_e715: candidate for 1020:e715 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1020's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1020_e715(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
