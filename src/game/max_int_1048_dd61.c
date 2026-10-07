/* max_int_1048_dd61: candidate for 1048:dd61 (38 bytes).  Verdict: EXACT.
 *
 * The larger of two ints (segment 1048's copy).
 */

#include "colonization.h"

int max_int_1048_dd61(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
