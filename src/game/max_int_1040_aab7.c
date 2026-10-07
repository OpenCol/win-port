/* max_int_1040_aab7: candidate for 1040:aab7 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1040's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1040_aab7(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
