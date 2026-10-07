/* max_int_1028_e350: candidate for 1028:e350 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1028's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1028_e350(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
