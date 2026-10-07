/* min_ptr_1030_f474: candidate for 1030:f474 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1030's copy.
 */

#include "colonization.h"

int * min_ptr_1030_f474(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
