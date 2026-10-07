/* max_int_1068_d81a: candidate for 1068:d81a (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1068's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1068_d81a(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
