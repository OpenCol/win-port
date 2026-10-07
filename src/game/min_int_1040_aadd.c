/* min_int_1040_aadd: candidate for 1040:aadd (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1040's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1040_aadd(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
