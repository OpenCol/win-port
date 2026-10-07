/* max_int_1038_e46c: candidate for 1038:e46c (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1038's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1038_e46c(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
