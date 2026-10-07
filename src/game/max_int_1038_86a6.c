/* max_int_1038_86a6: candidate for 1038:86a6 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1038's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1038_86a6(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
