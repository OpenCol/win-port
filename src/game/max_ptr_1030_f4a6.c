/* max_ptr_1030_f4a6: candidate for 1030:f4a6 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a tie); segment 1030's copy.
 */

#include "colonization.h"

int * max_ptr_1030_f4a6(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
