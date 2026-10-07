/* min_int_1058_be68: candidate for 1058:be68 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints, segment 1058's copy.  One of the small helpers the
 * compiler emits into every module that uses it.
 */

#include "colonization.h"

int min_int_1058_be68(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
