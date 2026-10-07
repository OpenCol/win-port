/* max_ptr_1018_b55a: candidate for 1018:b55a (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a tie); segment 1018's copy.
 */

#include "colonization.h"

int * max_ptr_1018_b55a(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
