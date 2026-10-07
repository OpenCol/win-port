/* max_int_1008_ce9c: candidate for 1008:ce9c (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1008's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1008_ce9c(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
