/* max_int_1058_c226: candidate for 1058:c226 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints, segment 1058's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int max_int_1058_c226(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
