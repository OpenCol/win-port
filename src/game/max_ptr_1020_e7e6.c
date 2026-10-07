/* max_ptr_1020_e7e6: candidate for 1020:e7e6 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a tie); segment 1020's copy.
 */

#include "colonization.h"

int * max_ptr_1020_e7e6(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
