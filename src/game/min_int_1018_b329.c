/* min_int_1018_b329: candidate for 1018:b329 (32 bytes).  Verdict: EXACT.
 *
 * The copy in segment 1018 of the smaller-of-two helper.
 */

#include "colonization.h"

int min_int_1018_b329(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
