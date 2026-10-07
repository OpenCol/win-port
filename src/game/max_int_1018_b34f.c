/* max_int_1018_b34f: candidate for 1018:b34f (32 bytes).  Verdict: EXACT.
 *
 * The copy in segment 1018 of the larger-of-two helper.
 */

#include "colonization.h"

int max_int_1018_b34f(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
