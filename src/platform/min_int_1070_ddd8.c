/* min_int_1070_ddd8: candidate for 1070:ddd8 (32 bytes).  Verdict: EXACT.
 *
 * The smaller of two ints (segment 1070's copy).
 */

#include "colonization.h"

int min_int_1070_ddd8(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}
