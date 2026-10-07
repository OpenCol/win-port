/* min_int_1008_ce76: candidate for 1008:ce76 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1008's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1008_ce76(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
