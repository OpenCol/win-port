/* min_int_1040_4483: candidate for 1040:4483 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1040's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1040_4483(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
