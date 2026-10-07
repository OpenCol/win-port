/* max_int_1060_63f4: candidate for 1060:63f4 (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints (segment 1060's copy).
 */

#include "colonization.h"

int max_int_1060_63f4(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
