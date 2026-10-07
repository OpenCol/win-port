/* min_int_1028_e376: candidate for 1028:e376 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1028's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1028_e376(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
