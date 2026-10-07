/* min_int_1010_e5d0: candidate for 1010:e5d0 (32 bytes).  Verdict: EXACT.
 *
 * The copy in segment 1010 of the smaller-of-two helper.
 */

#include "colonization.h"

int min_int_1010_e5d0(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
