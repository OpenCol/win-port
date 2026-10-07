/* min_ptr_1020_e818: candidate for 1020:e818 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1020's copy.
 */

#include "colonization.h"

int * min_ptr_1020_e818(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
