/* min_int_1060_641a: candidate for 1060:641a (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints (segment 1060's copy).
 */

#include "colonization.h"

int min_int_1060_641a(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
