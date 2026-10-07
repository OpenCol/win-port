/* max_int_1010_e632: candidate for 1010:e632 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1010's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1010_e632(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
