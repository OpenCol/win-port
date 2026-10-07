/* max_int_1020_a887: candidate for 1020:a887 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1020's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1020_a887(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
