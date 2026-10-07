/* min_int_1068_d7f4: candidate for 1068:d7f4 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1068's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1068_d7f4(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
