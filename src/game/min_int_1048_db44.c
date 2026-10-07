/* min_int_1048_db44: candidate for 1048:db44 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints (segment 1048's copy).
 */

#include "colonization.h"

int min_int_1048_db44(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
