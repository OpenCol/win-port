/* min_int_1050_91c4: candidate for 1050:91c4 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints (segment 1050's copy).
 */

#include "colonization.h"

int min_int_1050_91c4(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
