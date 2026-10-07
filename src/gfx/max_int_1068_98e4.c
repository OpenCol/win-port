/* max_int_1068_98e4: candidate for 1068:98e4 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1068's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1068_98e4(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
