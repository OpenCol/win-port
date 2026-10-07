/* max_int_1050_836b: candidate for 1050:836b (32 bytes).  Verdict: EXACT.
 *
 * The larger of two ints (segment 1050's copy).
 */

#include "colonization.h"

int max_int_1050_836b(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}
