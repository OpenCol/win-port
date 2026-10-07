/* max_ptr_1028_e5ff: candidate for 1028:e5ff (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a tie); segment 1028's copy.
 */

#include "colonization.h"

int * max_ptr_1028_e5ff(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
