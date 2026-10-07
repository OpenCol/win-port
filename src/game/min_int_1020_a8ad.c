/* min_int_1020_a8ad: candidate for 1020:a8ad (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1020's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1020_a8ad(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
